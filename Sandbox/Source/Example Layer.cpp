#include "Example Layer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Rendering\Core\Materials\Material.h"
#include "Flow\Assets\Materials\MaterialAsset.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"

#include "Content/ExampleWorldObject.h"
#include "Flow\Rendering\Core\Camera\Camera.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "Content\Wall.h"
#include "Content\Crate.h"

#include "Flow\Rendering\Core\Line.h"

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	//= Create Objects in level ===========

	WallObj = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Floor");

	//Example = Flow::Application::GetWorld()->SpawnWorldObject<ExampleWorldObject>("Testing World Object");

	//Crate1_ = Flow::Application::GetWorld()->SpawnWorldObject<Crate>("Crate1");
	//Crate2_ = Flow::Application::GetWorld()->SpawnWorldObject<Crate>("Crate2");
	//Crate3_ = Flow::Application::GetWorld()->SpawnWorldObject<Crate>("Crate3");
	//Crate4_ = Flow::Application::GetWorld()->SpawnWorldObject<Crate>("Crate4");
	//Crate5_ = Flow::Application::GetWorld()->SpawnWorldObject<Crate>("Crate5");
	//
	//Chair1_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Chair1");
	//Chair2_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Chair2");
	//Chair3_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Chair3");
	//Chair4_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Chair4");
	
	Table1_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Table1");
	Table2_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Table2");

	Revolver_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Revolver");
	//Blunderbuss_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Blunderbuss");
	//Shotgun_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Shotgun");
	//Rifle_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Rifle");
	
	Base = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Base");


	Actors.push_back(WallObj);
	//Actors.push_back(Example);
	Actors.push_back(Base);
	
	//Actors.push_back(Crate1_);
	//Actors.push_back(Crate2_);
	//Actors.push_back(Crate3_);
	//Actors.push_back(Crate4_);
	//Actors.push_back(Crate5_);
	//
	//Actors.push_back(Chair1_);
	//Actors.push_back(Chair2_);
	//Actors.push_back(Chair3_);
	//Actors.push_back(Chair4_);

	Actors.push_back(Table1_);
	Actors.push_back(Table2_);

	Actors.push_back(Revolver_);
	//Actors.push_back(Shotgun_);
	//Actors.push_back(Blunderbuss_);
	//Actors.push_back(Rifle_);
	


	//= Setup World Object properties ==========

	Flow::MeshAsset* Chair = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Chair");
	Flow::MeshAsset* Table = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Table");
	Flow::MeshAsset* Crate = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Crate");
	Flow::MeshAsset* Box = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Box");

	Flow::MeshAsset* Shotgun = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Shotgun");
	Flow::MeshAsset* Blunderbuss = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Blunderbuss");
	Flow::MeshAsset* Rifle = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Rifle");
	Flow::MeshAsset* Revolver = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Revolver");

	Flow::MaterialAsset* Props = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wabble_Props2");
	Flow::MaterialAsset* Weapons = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wabble_Weapons");
	Flow::MaterialAsset* Wood = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wood");
	Flow::MaterialAsset* Sand = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Sand");

	WallObj->GetMeshComponent()->SetMeshAndMaterial(Box, Sand->GetMaterial());
	Base->GetMeshComponent()->SetMeshAndMaterial(Box, Wood->GetMaterial());
	
	Table1_->GetMeshComponent()->SetMeshAndMaterial(Table, Props->GetMaterial());
	Table2_->GetMeshComponent()->SetMeshAndMaterial(Table, Props->GetMaterial());
	
	//Chair1_->GetMeshComponent()->SetMeshAndMaterial(Chair, Props->GetMaterial());
	//Chair2_->GetMeshComponent()->SetMeshAndMaterial(Chair, Props->GetMaterial());
	//Chair3_->GetMeshComponent()->SetMeshAndMaterial(Chair, Props->GetMaterial());
	//Chair4_->GetMeshComponent()->SetMeshAndMaterial(Chair, Props->GetMaterial());

	Revolver_->GetMeshComponent()->SetMeshAndMaterial(Revolver, Weapons->GetMaterial());
	//Shotgun_->GetMeshComponent()->SetMeshAndMaterial(Shotgun, Weapons->GetMaterial());
	//Blunderbuss_->GetMeshComponent()->SetMeshAndMaterial(Blunderbuss, Weapons->GetMaterial());
	//Rifle_->GetMeshComponent()->SetMeshAndMaterial(Rifle, Weapons->GetMaterial());

	//= Place everything in the level

	Light = std::make_shared<Flow::PointLight>(500.0f);
	
	WallObj->GetRootComponent()->SetRelativeScale(Vector(200, 10, 100));
	WallObj->GetRootComponent()->SetWorldLocation(Vector(0, -3, 0));

	//= Location

	Base->GetRootComponent()->SetWorldLocation(Vector(37.5f, 8.0f, 0.0f));
	
	//Crate1_->GetRootComponent()->SetWorldLocation(Vector(-20.0f, 10.0f, -10.0f));
	//Crate2_->GetRootComponent()->SetWorldLocation(Vector(-20.0f, 10.0f, 3.0f));
	//Crate3_->GetRootComponent()->SetWorldLocation(Vector(-20.0f, 10.0f, 16.0f));
	//Crate4_->GetRootComponent()->SetWorldLocation(Vector(-20.0f, 21.0f, -4.0f));
	//Crate5_->GetRootComponent()->SetWorldLocation(Vector(-20.0f, 21.0f, 10.0f));
	//
	//Chair1_->GetRootComponent()->SetWorldLocation(Vector(5.0f,  9.0f, 4.0f));
	//Chair2_->GetRootComponent()->SetWorldLocation(Vector(20.0f, 9.0f, 10.0f));
	//Chair3_->GetRootComponent()->SetWorldLocation(Vector(60.0f, 9.0f, 10.0f));
	//Chair4_->GetRootComponent()->SetWorldLocation(Vector(70.0f, 9.0f, 3.0f));
	
	Table1_->GetRootComponent()->SetWorldLocation(Vector(15.0f, 9.0f, 0.0f));
	Table2_->GetRootComponent()->SetWorldLocation(Vector(60.0f, 9.0f, 0.0f));

	Revolver_->GetRootComponent()->SetWorldLocation(Vector(14.0f, 20.0f, 0.0f));
	//Shotgun_->GetRootComponent()->SetWorldLocation(Vector(-15.0f, 20.0f, -13.0f));
	//Blunderbuss_->GetRootComponent()->SetWorldLocation(Vector(60.0f, 20.0f, 0.0f));
	//Rifle_->GetRootComponent()->SetWorldLocation(Vector(-20.0f, 32.0f, 0.0f));
															   	
	//= Rotation
	//Crate1_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	//Crate2_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	//Crate3_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	//Crate4_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	//Crate5_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	//
	//Chair1_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 300.0f));
	//Chair2_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 20.0f));
	//Chair3_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	//Chair4_->GetRootComponent()->SetRelativeRotation(Rotator(0.0f, 0.0f, 70.0f));

	Table1_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	Table2_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));

	Revolver_->GetRootComponent()->SetWorldRotation(Rotator(90.0f, 0.0f, 300.0f));
	//Shotgun_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 170.0f));
	//Blunderbuss_->GetRootComponent()->SetWorldRotation(Rotator(90.0f, 140.0f, 0.0f));
	//Rifle_->GetRootComponent()->SetRelativeRotation(Rotator(90.0f, 90.0f, 0.0f));
	
	//= Scale
	
	Base->GetRootComponent()->SetWorldScale(Vector(50.0f, 1.0f, 30.0f));

	//Chair1_->InitPhysics(false);
	//Chair2_->InitPhysics(false);
	//Chair3_->InitPhysics(false);
	//Chair4_->InitPhysics(false);

	Revolver_->InitPhysics(true);
	//Shotgun_->InitPhysics(true);
	//Blunderbuss_->InitPhysics(true);
	//Rifle_->InitPhysics(true);
}

ExampleLayer::~ExampleLayer()
{
	Actors.clear();
}

void ExampleLayer::OnUpdate(float DeltaTime)
{
	int Count = 0;
	Flow::Renderer::BeginScene();

	Light->BindLight(Flow::RenderCommand::GetCamera().GetMatrix());
	//Flow::Line::DrawLine(Vector(0.0f, 50.0f, 0.0f), Vector(200.0f, 50.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f));

	Flow::RenderCommand::GetCamera().Tick(DeltaTime);

	for (auto& Actor : Actors)
	{
		Actor->Render();
	}

	Flow::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	Flow::RenderCommand::GetCamera().RenderIMGUIWindow();

	Flow::AssetSystem::RenderDebugWindow(true);
	Light->RenderControlWindow();
}

void ExampleLayer::OnAttach()
{
	Layer::OnAttach();
}
