#include "AGDLayer.h"

#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Rendering\Renderer.h"

#include "Flow\GameFramework\WorldObject.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"

#include "Content\WorldObjects\MeshWorldObject.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"

#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Assets\Textures\TextureAsset.h"
#include "Flow\Assets\Materials\MaterialAsset.h"
#include "Flow\Assets\Materials\Mat_TexturedPhong.h"
#include "Flow\Assets\Materials\Mat_LitColour.h"

#include "Content\WorldObjects\PlayerPlane.h"
#include "Flow\Rendering\Core\Sprite.h"

#include "ThirdParty\ImGui\imgui.h"

#include "Content/OpenCVTesting.h"

#include "Flow\Helper\Instrumentation.h"

#include "Content\World\WorldGenerator.h"
#include "Content\World\ObstacleWall.h"
#include "Flow\Rendering\Core\Bindables\IndexBuffer.h"

#include "Flow\Rendering\Core\Camera\BasicCamera.h"
#include "Flow\GameFramework\Controllers\PlayerController.h"

#include "Flow\Rendering\Core\Lights\PointLight.h"

#include "Flow\GameFramework\World.h"

OpenCVTesting* AGDLayer::CVTesting_ = new OpenCVTesting();

AGDLayer::AGDLayer()
	: Layer("Advance Games Dev Example"), Generator_(new WorldGenerator())
{
	//Aquire all assets
	Flow::MeshAsset* PlaneMesh = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("CharacterPlane");
	Flow::MeshAsset* Box = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Box");
	Flow::MeshAsset* Tube = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Tube");
	Flow::MaterialAsset* Wood = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wood");
	Flow::MeshAsset* Map = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("MapTest");
	Flow::MaterialAsset* Brown = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_FlatColour_Brown");
	Flow::MaterialAsset* LitGrey = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_LitGrey");
	
	//Create required materials
	Flow::AssetSystem::CreateMaterial<Flow::Mat_TexturedPhong>("Mat_CharacterPlane");
	Flow::Mat_TexturedPhong* PlaneMat = static_cast<Flow::Mat_TexturedPhong*>(Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_CharacterPlane")->GetMaterial());
	PlaneMat->SetTexture("CharacterPlaneTexture");
	PlaneMat->SetPixelShader("TexturedPS");
	PlaneMat->SetVertexShader("TexturedVS");

	//Initialise World
	Light_ = std::make_shared<Flow::PointLight>(500.0f);

	Player_ = Flow::Application::GetWorld()->SpawnWorldObject<PlayerPlane>("Player Plane");
	Player_->SetPhysicsMode(Flow::PhysicsMode::Dynamic);
	WorldObjects_.push_back(Player_);

	Player_->GetRootComponent()->SetRelativePosition(Vector(0.0f, -100.0f, -500.0));

	Base_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Base");
	Base_->SetPhysicsMode(Flow::PhysicsMode::Static);
	static_cast<Flow::StaticMeshComponent*>(Base_->GetRootComponent())->SetMeshAndMaterial(Box, Wood->GetMaterial());
	Base_->GetRootComponent()->SetWorldScale(Vector(400.0f, 0.1f, 200.0f));
	WorldObjects_.push_back(Base_);


	TestCube_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("CubeTest");
	TestCube_->SetPhysicsMode(Flow::PhysicsMode::Static);
	static_cast<Flow::StaticMeshComponent*>(TestCube_->GetRootComponent())->SetMeshAndMaterial(PlaneMesh, PlaneMat);
	static_cast<Flow::StaticMeshComponent*>(TestCube_->GetRootComponent())->UseCompoundCollision_ = true;
	static_cast<Flow::StaticMeshComponent*>(TestCube_->GetRootComponent())->PlaneCollision = true;
	TestCube_->GetRootComponent()->SetWorldScale(Vector(1.0f, 1.0f, 1.0f));
	TestCube_->GetRootComponent()->SetWorldPosition(Vector(0.0f, 50.0f, -400.0f));
	WorldObjects_.push_back(TestCube_);

	Camera_ = new Flow::BasicCamera();

	//Initialise OpenCV data
	if (UseOpenCV)
	{
		CVTesting_->Initialise(false);
		CVTesting_->SetFlatHeight(500.0f);
	}

	TrackingTimer.Mark();
}

AGDLayer::~AGDLayer()
{
}

void AGDLayer::OnUpdate(float DeltaTime)
{
	PROFILE_FUNCTION();

	int Count = 0;
	Flow::Renderer::BeginScene();

	Light_->BindLight(Flow::RenderCommand::GetCamera().GetView());
	bool Paused = Flow::Application::GetApplication().IsGamePaused();

	if (UseOpenCV)
	{
		//Initialise the trackers after delay
		if (!Paused && !CVTesting_->Started && TrackingTimer.Peek() > 5.0f)
		{
			CVTesting_->StartTracker(ETracker::Hand_Left, IntVector2D(950, 500));
			CVTesting_->StartTracker(ETracker::Hand_Right, IntVector2D(600, 500));
			CVTesting_->Started = true;
		}

		//Update the openCV interface and the player
		CVTesting_->Update();
		if(CVTesting_->Started && !Paused)
			Player_->OpenCVUpdate(CVTesting_->CalculateAngle(), CVTesting_->GetHeightDeviation());		

		//Update the camera
		Camera_->SetPosition(Player_->GetRootComponent()->GetRelativePosition() + Vector(0.0f, 10.0f, -20.0f));
		Camera_->CacheCameraMatrices();
	}

	//Update world Generator
	if (!Paused)
	{
		PROFILE_CURRENT_SCOPE("World Generator Update");
		if (Generator_->Update(DeltaTime))
			Generator_->CreateWall(Flow::Application::GetWorld());
	}

	//Draw the world
	{
		PROFILE_CURRENT_SCOPE("Draw World");

		for (auto& Actor : WorldObjects_)
		{
			Actor->Render();
		}

		for (auto& Object : GeneratedWorld)
		{
			Object->Render();
		}

		Generator_->Render();
	}

	Flow::Renderer::EndScene();
}

void AGDLayer::OnImGuiRender()
{
	Flow::Application::GetWorld()->DrawWorldSettings();

	if (UseOpenCV)
		CVTesting_->DrawOpenCVControls();

	Flow::MaterialAsset* LitColour = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_LitGrey");

	//if(ImGui::Begin("Test"))
	//{
	//	if(ImGui::ColorPicker3("Cube Colour", (float*)&CubeColour_))
	//		static_cast<Flow::Mat_LitColour*>(LitColour->GetMaterial())->
	//		SetColour(Vector(CubeColour_[0], CubeColour_[1], CubeColour_[2]));
	//}
	//ImGui::End();

	//if(ImGui::Begin("Light Settings"))
	//{
	//	if (ImGui::Checkbox("Use Rotated vector", &UseRotatedLightVector))
	//		static_cast<Flow::Mat_LitColour*>(LitColour->GetMaterial())->SetRotated(UseRotatedLightVector);
	//}
	//ImGui::End();
}

void AGDLayer::OnAttach()
{
}

void AGDLayer::PostBeginPlay()
{
	Flow::Application::GetWorld()->GetLocalController()->SetCamera(Camera_);
}

OpenCVTesting* AGDLayer::GetCVTester()
{
	return CVTesting_;
}
