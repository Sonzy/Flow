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

#include "Content\WorldObjects\PlayerPlane.h"
#include "Flow\Rendering\Core\Sprite.h"

#include "ThirdParty\ImGui\imgui.h"

#include "Flow\Misc\OpenCVTesting.h"

#include "Flow\Helper\Instrumentation.h"

AGDLayer::AGDLayer()
	: Layer("Advance Games Dev Example")
{
	Flow::MeshAsset* PlaneMesh = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("CharacterPlane");
	Flow::MeshAsset* Box = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Box");
	Flow::MaterialAsset* Wood = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wood");

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
	WorldObjects_.push_back(Player_);


	Base_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Base");
	static_cast<Flow::StaticMeshComponent*>(Base_->GetRootComponent())->SetMeshAndMaterial(Box, Wood->GetMaterial());
	Base_->GetRootComponent()->SetWorldScale(Vector(400.0f, 0.1f, 200.0f));
	WorldObjects_.push_back(Base_);


	Flow::TextureAsset* Texture = Flow::AssetSystem::GetAsset<Flow::TextureAsset>("TestSprite");
	//Sprite_ = std::make_shared<Flow::Sprite>(Texture);
	//
	//Sprite_->SetScale(Vector(512.0f, 512.0f, 1.0f));
	//Sprite_->SetPosition(Vector(0.0f, 0.0f, 0.0f));

	CVTesting_ = std::make_shared<Flow::OpenCVTesting>();
}

AGDLayer::~AGDLayer()
{
}

void AGDLayer::OnUpdate(float DeltaTime)
{
	PROFILE_FUNCTION();

	int Count = 0;
	Flow::Renderer::BeginScene();


	//CVTesting_->Update();
	//Sprite_->Update();
	//Flow::Renderer::Submit(Sprite_.get());
	for (auto& Actor : WorldObjects_)
	{
		Actor->Render();
	}

	//Flow::RenderCommand::DisableDepth();

	//Flow::RenderCommand::EnableDepth();

	Flow::Renderer::EndScene();
}

void AGDLayer::OnImGuiRender()
{
	Flow::Application::GetWorld()->DrawWorldSettings();
	//Sprite_->ControlWindow();

	//CVTesting_->RenderToIMGUI();

	if( ImGui::Begin("SpriteDrawing")) 
	{
		ImGui::Checkbox("Draw Last", &DrawLast);
	}
	ImGui::End();
}

void AGDLayer::OnAttach()
{
}
