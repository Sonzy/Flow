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

OpenCVTesting* AGDLayer::CVTesting_ = new OpenCVTesting();

AGDLayer::AGDLayer()
	: Layer("Advance Games Dev Example"), Generator_(new WorldGenerator())
{
	Flow::MeshAsset* PlaneMesh = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("CharacterPlane");
	Flow::MeshAsset* Box = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Box");
	Flow::MeshAsset* Tube = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Tube");
	Flow::MaterialAsset* Wood = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wood");
	Flow::MeshAsset* Map = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("MapTest");
	Flow::MaterialAsset* Brown = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_FlatColour_Brown");
	Flow::MaterialAsset* LitGrey = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_LitGrey");
	
	Flow::AssetSystem::CreateMaterial<Flow::Mat_TexturedPhong>("Mat_CharacterPlane");
	Flow::Mat_TexturedPhong* PlaneMat = static_cast<Flow::Mat_TexturedPhong*>(Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_CharacterPlane")->GetMaterial());
	PlaneMat->SetTexture("CharacterPlaneTexture");
	PlaneMat->SetPixelShader("TexturedPS");
	PlaneMat->SetVertexShader("TexturedVS");


	//PlaneTest_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Plane Character Test");
	//WorldObjects_.push_back(PlaneTest_);
	//PlaneTest_->GetMeshComponent()->SetMeshAndMaterial(PlaneMesh, PlaneMat);
	//PlaneTest_->GetMeshComponent()->SetWorldPosition(Vector(0.0f, 20.0f, 0.0f));

	Player_ = Flow::Application::GetWorld()->SpawnWorldObject<PlayerPlane>("Player Plane");
	Player_->SetPhysicsMode(Flow::PhysicsMode::Dynamic);
	WorldObjects_.push_back(Player_);
	//Player_->GetRootComponent()->SetRelativePosition(Vector(0.0f, 10.0f, -20.0f));
	Player_->GetRootComponent()->SetRelativePosition(Vector(0.0f, 10.0f, -500.0));
	//static_cast<PlayerPlane*>(Player_.get())->ToggleWASDMode();


	Base_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Base");
	Base_->SetPhysicsMode(Flow::PhysicsMode::Static);
	static_cast<Flow::StaticMeshComponent*>(Base_->GetRootComponent())->SetMeshAndMaterial(Box, Wood->GetMaterial());
	Base_->GetRootComponent()->SetWorldScale(Vector(400.0f, 0.1f, 200.0f));
	WorldObjects_.push_back(Base_);

	//Map_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Map");
	//static_cast<Flow::StaticMeshComponent*>(Map_->GetRootComponent())->SetMeshAndMaterial(Map, LitGrey->GetMaterial());
	//WorldObjects_.push_back(Map_);

	//TestCube_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("CubeTest");
	//TestCube_->SetPhysicsMode(Flow::PhysicsMode::Static);
	//static_cast<Flow::StaticMeshComponent*>(TestCube_->GetRootComponent())->SetMeshAndMaterial(Tube, LitGrey->GetMaterial());
	//TestCube_->GetRootComponent()->SetWorldScale(Vector(1.0f, 1.0f, 1.0f));
	//TestCube_->GetRootComponent()->SetWorldPosition(Vector(10.0f, 50.0f, 10.0f));
	////WorldObjects_.push_back(TestCube_);

	Wall_ = Flow::Application::GetWorld()->SpawnWorldObject<ObstacleWall>("Walltesting");
	Wall_->GetRootComponent()->SetWorldScale(Vector(1.0f, 1.0f, 1.0f));
	Wall_->GetRootComponent()->SetWorldPosition(Vector(10.0f, 50.0f, 10.0f));
	WorldObjects_.push_back(Wall_);

	Camera_ = new Flow::BasicCamera();
	Camera_->Translate({0.0f, 0.0f, -20.0f});

	Flow::TextureAsset* Texture = Flow::AssetSystem::GetAsset<Flow::TextureAsset>("TestSprite");
	//Sprite_ = std::make_shared<Flow::Sprite>(Texture);
	//
	//Sprite_->SetScale(Vector(512.0f, 512.0f, 1.0f));
	//Sprite_->SetPosition(Vector(0.0f, 0.0f, 0.0f));

	//Create the world
	//GeneratedWorld = WorldGenerator::CreateWorld(IntVector2D(10), 500.0f, 4, 30.0f, Vector(300.0f, 4000.0f, 300.0f));

	if(UseOpenCV)
		CVTesting_->Initialise();
}

AGDLayer::~AGDLayer()
{
}

void AGDLayer::OnUpdate(float DeltaTime)
{
	PROFILE_FUNCTION();

	int Count = 0;
	Flow::Renderer::BeginScene();

	Camera_->Update(DeltaTime);

	if (UseOpenCV)
		CVTesting_->Update();

	if(UseCVControls)
	Player_->GetRootComponent()->SetWorldRotation(
		Rotator(0.0f, CVTesting_->CalculateAngle() + 180.0f, 0.0f));

	if (Generator_->Update(DeltaTime))
		Generator_->CreateWall(Flow::Application::GetWorld());

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

	if(ImGui::Begin("Light Settings"))
	{
		if (ImGui::Checkbox("Use Rotated vector", &UseRotatedLightVector))
			static_cast<Flow::Mat_LitColour*>(LitColour->GetMaterial())->SetRotated(UseRotatedLightVector);
	}
	ImGui::End();
}

void AGDLayer::OnAttach()
{
}

void AGDLayer::PostBeginPlay()
{
	//Flow::Application::GetWorld()->GetLocalController()->SetCamera(Camera_);
}

OpenCVTesting* AGDLayer::GetCVTester()
{
	return CVTesting_;
}
