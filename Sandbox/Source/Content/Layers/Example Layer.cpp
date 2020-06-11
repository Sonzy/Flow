#include "Example Layer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Rendering\Core\Materials\Material.h"
#include "Flow\Assets\Materials\MaterialAsset.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"

#include "Content\WorldObjects\ExampleWorldObject.h"
#include "Flow\Rendering\Core\Camera\Camera.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "Content\Wall.h"
#include "Content\Crate.h"

#include "Flow\Rendering\Core\Line.h"

#include "Flow\GameFramework\Components\CameraComponent.h"

#include "Flow/GameFramework/Actors/CameraActor.h"
#include "Flow/GameFramework/Actors/SkyboxActor.h"

#include "Flow/GameFramework/Controllers/Controller.h"

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	//= Create Objects in level ===========

	WallObj = Application::GetWorld()->SpawnActor<MeshWorldObject>("Floor");
	TestCube = Application::GetWorld()->SpawnActor<MeshWorldObject>("TestCube");

	Crate1_ = Application::GetWorld()->SpawnActor<Crate>("Crate1");
	Crate2_ = Application::GetWorld()->SpawnActor<Crate>("Crate2");
	Crate3_ = Application::GetWorld()->SpawnActor<Crate>("Crate3");
	Crate4_ = Application::GetWorld()->SpawnActor<Crate>("Crate4");
	Crate5_ = Application::GetWorld()->SpawnActor<Crate>("Crate5");
	
	Chair1_ = Application::GetWorld()->SpawnActor<MeshWorldObject>("Chair1");
	Chair2_ = Application::GetWorld()->SpawnActor<MeshWorldObject>("Chair2");
	Chair3_ = Application::GetWorld()->SpawnActor<MeshWorldObject>("Chair3");
	Chair4_ = Application::GetWorld()->SpawnActor<MeshWorldObject>("Chair4");
	
	Table1_ = Application::GetWorld()->SpawnActor<MeshWorldObject>("Table1");
	Table2_ = Application::GetWorld()->SpawnActor<MeshWorldObject>("Table2");

	Revolver_ = Application::GetWorld()->SpawnActor<MeshWorldObject>("Revolver");
	Blunderbuss_ = Application::GetWorld()->SpawnActor<MeshWorldObject>("Blunderbuss");
	Shotgun_ = Application::GetWorld()->SpawnActor<MeshWorldObject>("Shotgun");
	Rifle_ = Application::GetWorld()->SpawnActor<MeshWorldObject>("Rifle");
	
	Base = Application::GetWorld()->SpawnActor<MeshWorldObject>("Base");

	_CameraActor = Application::GetWorld()->SpawnActor<CameraActor>("Camera Actor");
	_Skybox = Application::GetWorld()->SpawnActor<SkyboxActor>("Skybox Actor");

	Actors.push_back(WallObj);
	//Actors.push_back(Example);
	Actors.push_back(Base);
	Actors.push_back(TestCube);
	
	Actors.push_back(Crate1_);
	Actors.push_back(Crate2_);
	Actors.push_back(Crate3_);
	Actors.push_back(Crate4_);
	Actors.push_back(Crate5_);
	
	Actors.push_back(Chair1_);
	Actors.push_back(Chair2_);
	Actors.push_back(Chair3_);
	Actors.push_back(Chair4_);

	Actors.push_back(Table1_);
	Actors.push_back(Table2_);

	Actors.push_back(Revolver_);
	Actors.push_back(Shotgun_);
	Actors.push_back(Blunderbuss_);
	Actors.push_back(Rifle_);

	Actors.push_back(_CameraActor);
	Actors.push_back(_Skybox);
	


	//= Setup World Object properties ==========

	MeshAsset* Chair = AssetSystem::GetAsset<MeshAsset>("Wabble_Chair");
	MeshAsset* Table = AssetSystem::GetAsset<MeshAsset>("Wabble_Table");
	MeshAsset* Crate = AssetSystem::GetAsset<MeshAsset>("Wabble_Crate");
	MeshAsset* Box = AssetSystem::GetAsset<MeshAsset>("Box");

	MeshAsset* Shotgun = AssetSystem::GetAsset<MeshAsset>("Wabble_Shotgun");
	MeshAsset* ShotgunSmooth = AssetSystem::GetAsset<MeshAsset>("Wabble_Shotgun_Smooth");
	MeshAsset* ShotgunTest = AssetSystem::GetAsset<MeshAsset>("Wabble_Shotgun_Test");
	MeshAsset* Blunderbuss = AssetSystem::GetAsset<MeshAsset>("Wabble_Blunderbuss");
	MeshAsset* Rifle = AssetSystem::GetAsset<MeshAsset>("Wabble_Rifle");
	MeshAsset* Revolver = AssetSystem::GetAsset<MeshAsset>("Wabble_Revolver");

	MaterialAsset* Props = AssetSystem::GetAsset<MaterialAsset>("Mat_Wabble_Props");
	MaterialAsset* Weapons = AssetSystem::GetAsset<MaterialAsset>("Mat_Wabble_Weapons");
	MaterialAsset* Wood = AssetSystem::GetAsset<MaterialAsset>("Mat_Wood");
	MaterialAsset* Sand = AssetSystem::GetAsset<MaterialAsset>("Mat_Sand");
	MaterialAsset* SkyCube = AssetSystem::GetAsset<MaterialAsset>("Mat_SkyCube");

	WallObj->GetMeshComponent()->SetMeshAndMaterial(Box, Sand->GetMaterial());
	Base->GetMeshComponent()->SetMeshAndMaterial(Box, Wood->GetMaterial());
	TestCube->GetMeshComponent()->SetMeshAndMaterial(Box, SkyCube->GetMaterial());
	//Base->GetMeshComponent()->EnableOutlineDrawing(true);
	//Base->GetMeshComponent()->RefreshBinds();
	
	Table1_->GetMeshComponent()->SetMeshAndMaterial(Table, Props->GetMaterial());
	Table2_->GetMeshComponent()->SetMeshAndMaterial(Table, Props->GetMaterial());
	
	Chair1_->GetMeshComponent()->SetMeshAndMaterial(Chair, Props->GetMaterial());
	Chair2_->GetMeshComponent()->SetMeshAndMaterial(Chair, Props->GetMaterial());
	Chair3_->GetMeshComponent()->SetMeshAndMaterial(Chair, Props->GetMaterial());
	Chair4_->GetMeshComponent()->SetMeshAndMaterial(Chair, Props->GetMaterial());

	Revolver_->GetMeshComponent()->SetMeshAndMaterial(Revolver, Weapons->GetMaterial());
	Shotgun_->GetMeshComponent()->SetMeshAndMaterial(Shotgun, Weapons->GetMaterial());
	Blunderbuss_->GetMeshComponent()->SetMeshAndMaterial(Blunderbuss, Weapons->GetMaterial());
	Rifle_->GetMeshComponent()->SetMeshAndMaterial(Rifle, Weapons->GetMaterial());

	//= Place everything in the level

	Light = std::make_shared<PointLight>(500.0f);
	
	//= Location

	Base->GetRootComponent()->SetWorldPosition(Vector(3.5f, 0.0f, 0.0f));
	WallObj->GetRootComponent()->SetWorldPosition(Vector(0, -1.0f, 0));
	TestCube->GetRootComponent()->SetWorldPosition(Vector(0, 5.0f, 0));
	
	Crate1_->GetRootComponent()->SetWorldPosition(Vector(-2.0f, 0.5f, -1.0f));
	Crate2_->GetRootComponent()->SetWorldPosition(Vector(-2.0f, 0.5f, 0.3f));
	Crate3_->GetRootComponent()->SetWorldPosition(Vector(-2.0f, 0.5f, 1.6f));
	Crate4_->GetRootComponent()->SetWorldPosition(Vector(-2.0f, 1.5f, -0.4f));
	Crate5_->GetRootComponent()->SetWorldPosition(Vector(-2.0f, 1.5f, 1.0f));
	
	Chair1_->GetRootComponent()->SetWorldPosition(Vector(0.5f,  0.4f, 0.4f));
	Chair2_->GetRootComponent()->SetWorldPosition(Vector(2.0f, 0.4f, 1.0f));
	Chair3_->GetRootComponent()->SetWorldPosition(Vector(5.0f, 0.4f, 1.0f));
	Chair4_->GetRootComponent()->SetWorldPosition(Vector(6.0f, 0.4f, 0.3f));
	
	Table1_->GetRootComponent()->SetWorldPosition(Vector(1.5f, 0.4f, 0.0f));
	Table2_->GetRootComponent()->SetWorldPosition(Vector(5.0f, 0.4f, 0.0f));

	Revolver_->GetRootComponent()->SetWorldPosition(Vector(1.5f, 0.8f, 0.0f));
	Shotgun_->GetRootComponent()->SetWorldPosition(Vector(-1.5f, 1.2f, -1.3f));
	Blunderbuss_->GetRootComponent()->SetWorldPosition(Vector(5.0f, 0.8f, 0.0f));
	Rifle_->GetRootComponent()->SetWorldPosition(Vector(-2.0f, 2.2f, 0.0f));
															   	
	//= Rotation
	Crate1_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	Crate2_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	Crate3_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	Crate4_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	Crate5_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	
	Chair1_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 300.0f));
	Chair2_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 20.0f));
	Chair3_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	Chair4_->GetRootComponent()->SetRelativeRotation(Rotator(0.0f, 0.0f, 70.0f));

	Table1_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));
	Table2_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 0.0f));

	Revolver_->GetRootComponent()->SetWorldRotation(Rotator(90.0f, 0.0f, 300.0f));
	Shotgun_->GetRootComponent()->SetWorldRotation(Rotator(0.0f, 0.0f, 170.0f));
	Blunderbuss_->GetRootComponent()->SetWorldRotation(Rotator(90.0f, 140.0f, 0.0f));
	Rifle_->GetRootComponent()->SetRelativeRotation(Rotator(90.0f, 90.0f, 0.0f));
	
	//= Scale
	
	Base->GetRootComponent()->SetWorldScale(Vector(4.0f, 0.1f, 2.0f));
	WallObj->GetRootComponent()->SetRelativeScale(Vector(20.0f, 1.0f, 10.0f));

	Base->GetRootComponent()->SetSimulatePhysics(false);
	WallObj->GetRootComponent()->SetSimulatePhysics(false);
	TestCube->GetRootComponent()->SetSimulatePhysics(true);

	Crate1_->GetRootComponent()->SetSimulatePhysics(true);
	Crate2_->GetRootComponent()->SetSimulatePhysics(true);
	Crate3_->GetRootComponent()->SetSimulatePhysics(true);
	Crate4_->GetRootComponent()->SetSimulatePhysics(true);
	Crate5_->GetRootComponent()->SetSimulatePhysics(true);

	Chair1_->GetRootComponent()->SetSimulatePhysics(true);
	Chair2_->GetRootComponent()->SetSimulatePhysics(true);
	Chair3_->GetRootComponent()->SetSimulatePhysics(true);
	Chair4_->GetRootComponent()->SetSimulatePhysics(true);

	Table1_->GetRootComponent()->SetSimulatePhysics(true);
	Table2_->GetRootComponent()->SetSimulatePhysics(true);

	Revolver_->GetRootComponent()->SetSimulatePhysics(true);
	Shotgun_->GetRootComponent()->SetSimulatePhysics(true);
	Blunderbuss_->GetRootComponent()->SetSimulatePhysics(true);
	Rifle_->GetRootComponent()->SetSimulatePhysics(true);
}

ExampleLayer::~ExampleLayer()
{
	Actors.clear();
}

void ExampleLayer::BeginPlay()
{
	Application::GetWorld()->GetLocalController()->ControlObject(_CameraActor.get());
}

void ExampleLayer::OnUpdate(float DeltaTime)
{
	PROFILE_FUNCTION();

	int Count = 0;
	Renderer::BeginScene();

	Light->BindLight(RenderCommand::GetMainCamera()->GetViewMatrix());
	//Line::DrawLine(Vector(0.0f, 50.0f, 0.0f), Vector(200.0f, 50.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f));

	//RenderCommand::GetCamera().Tick(DeltaTime);

	//TODO: Move this stuff
	for (auto& Actor : Actors)
	{
		Actor->Render();
	}

	Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender(bool DrawEditor)
{
	if (DrawEditor)
	{
		AssetSystem::RenderDebugWindow(true);
		Light->RenderControlWindow();
	}
}

void ExampleLayer::OnAttach()
{
	Layer::OnAttach();
}
