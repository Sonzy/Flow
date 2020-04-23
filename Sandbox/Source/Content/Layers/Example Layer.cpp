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

#include "Flow\Assets\Materials\Mat_FlatColour.h"
#include "Flow\Assets\Materials\Mat_TexturedPhong.h"
#include "Flow\Assets\Meshes\MeshAsset.h"

#include "Content\WorldObjects\PlayerPlane.h"

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	LoadAssets();
	//= Create Objects in level ===========

	WallObj = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Floor");
	TestCube = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("TestCube");

	Crate1_ = Flow::Application::GetWorld()->SpawnWorldObject<Crate>("Crate1");
	Crate2_ = Flow::Application::GetWorld()->SpawnWorldObject<Crate>("Crate2");
	Crate3_ = Flow::Application::GetWorld()->SpawnWorldObject<Crate>("Crate3");
	Crate4_ = Flow::Application::GetWorld()->SpawnWorldObject<Crate>("Crate4");
	Crate5_ = Flow::Application::GetWorld()->SpawnWorldObject<Crate>("Crate5");
	
	Chair1_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Chair1");
	Chair2_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Chair2");
	Chair3_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Chair3");
	Chair4_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Chair4");
	
	Table1_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Table1");
	Table2_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Table2");

	Revolver_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Revolver");
	Blunderbuss_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Blunderbuss");
	Shotgun_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Shotgun");
	Rifle_ = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Rifle");
	
	Base = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>("Base");

	Player_ = Flow::Application::GetWorld()->SpawnWorldObject<PlayerPlane>("Player Plane");
	Actors.push_back(Player_);
	Player_->GetRootComponent()->SetRelativePosition(Vector(0.0f, 10.0f, -20.0f));


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
	


	//= Setup World Object properties ==========

	Flow::MeshAsset* Chair = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Chair");
	Flow::MeshAsset* Table = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Table");
	Flow::MeshAsset* Crate = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Crate");
	Flow::MeshAsset* Box = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Box");

	Flow::MeshAsset* Shotgun = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Shotgun");
	Flow::MeshAsset* ShotgunSmooth = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Shotgun_Smooth");
	Flow::MeshAsset* ShotgunTest = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Shotgun_Test");
	Flow::MeshAsset* Blunderbuss = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Blunderbuss");
	Flow::MeshAsset* Rifle = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Rifle");
	Flow::MeshAsset* Revolver = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Revolver");

	Flow::MaterialAsset* Props = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wabble_Props");
	Flow::MaterialAsset* Weapons = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wabble_Weapons");
	Flow::MaterialAsset* Wood = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wood");
	Flow::MaterialAsset* Sand = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Sand");
	Flow::MaterialAsset* SkyCube = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_SkyCube");

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

	Light = std::make_shared<Flow::PointLight>(500.0f);
	
	//= Location

	Base->GetRootComponent()->SetWorldPosition(Vector(3.5f, 0.0f, 0.0f));
	WallObj->GetRootComponent()->SetWorldPosition(Vector(0, -1.0f, 0));
	TestCube->GetRootComponent()->SetWorldPosition(Vector(0, 5.0f, 0));
	
	Crate1_->GetRootComponent()->SetWorldPosition(Vector(-2.0f, 0.5f, -1.0f));
	Crate2_->GetRootComponent()->SetWorldPosition(Vector(-2.0f, 0.5f, 0.3f));
	Crate3_->GetRootComponent()->SetWorldPosition(Vector(-2.0f, 0.5f, 1.6f));
	Crate4_->GetRootComponent()->SetWorldPosition(Vector(-2.0f, 1.5f, -0.4f));
	Crate5_->GetRootComponent()->SetWorldPosition(Vector(-2.0f, 10.5f, 1.0f));
	
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

	Chair1_->InitPhysics(false);
	Chair2_->InitPhysics(false);
	Chair3_->InitPhysics(false);
	Chair4_->InitPhysics(false);

	Revolver_->InitPhysics(true);
	Shotgun_->InitPhysics(true);
	Blunderbuss_->InitPhysics(true);
	Rifle_->InitPhysics(true);
}

ExampleLayer::~ExampleLayer()
{
	Actors.clear();
}

void ExampleLayer::OnUpdate(float DeltaTime)
{
	int Count = 0;
	Flow::Renderer::BeginScene();

	Light->BindLight(Flow::RenderCommand::GetCamera().GetViewMatrix());
	//Flow::Line::DrawLine(Vector(0.0f, 50.0f, 0.0f), Vector(200.0f, 50.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f));

	//Flow::RenderCommand::GetCamera().Tick(DeltaTime);

	for (auto& Actor : Actors)
	{
		Actor->Render();
	}

	Flow::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	//Flow::RenderCommand::GetCamera().RenderIMGUIWindow();

	Flow::AssetSystem::RenderDebugWindow(true);
	Light->RenderControlWindow();
}

void ExampleLayer::OnAttach()
{
	Layer::OnAttach();
}

void ExampleLayer::LoadAssets()
{
	//TODO: Load assets somewhere
	//= Models =
	Flow::AssetSystem::LoadAsset("Box2", "Flow/Assets/Models/Box2.obj");
	Flow::AssetSystem::LoadAsset("WeirdBox", "Flow/Assets/Models/WeirdBox.obj"); 
	Flow::AssetSystem::LoadAsset("Hat_FancyMan", "Flow/Assets/Models/Hat_FancyMan.obj");
	Flow::AssetSystem::LoadAsset("Plane", "Flow/Assets/Models/Plane.obj");
	Flow::AssetSystem::LoadAsset("Train", "Flow/Assets/Models/Train.FBX");
	Flow::AssetSystem::LoadAsset("Hat_Sherif", "Flow/Assets/Models/Hat_Sherif.obj");
	Flow::AssetSystem::LoadAsset("Sphere", "Flow/Assets/Models/Sphere.obj");
	Flow::AssetSystem::LoadAsset("SelectionGizmo", "Flow/Assets/Models/SelectionGizmo.obj");
	Flow::AssetSystem::LoadAsset("Wabble_Crate", "Flow/Assets/Models/Wabble_Crate.obj");
	Flow::AssetSystem::LoadAsset("Wabble_Chair", "Flow/Assets/Models/Wabble_Chair.obj");
	Flow::AssetSystem::LoadAsset("Wabble_Table", "Flow/Assets/Models/Wabble_Table.obj");
	Flow::AssetSystem::LoadAsset("Wabble_Revolver", "Flow/Assets/Models/Wabble_Revolver.obj");
	Flow::AssetSystem::LoadAsset("Wabble_Rifle", "Flow/Assets/Models/Wabble_Rifle.obj");
	Flow::AssetSystem::LoadAsset("Wabble_Shotgun", "Flow/Assets/Models/Wabble_Shotgun.obj");
	Flow::AssetSystem::LoadAsset("Wabble_Blunderbuss", "Flow/Assets/Models/Wabble_Blunderbuss.obj");

	//= Collision ======================

	Flow::AssetSystem::LoadAsset("Wabble_Table_Collision", "Flow/Assets/Collision/Collision_Wabble_Table.obj");
	Flow::AssetSystem::LoadAsset("Wabble_Chair_Collision", "Flow/Assets/Collision/Collision_Wabble_Chair.obj");
	Flow::AssetSystem::LoadAsset("Wabble_Shotgun_Collision", "Flow/Assets/Collision/Collision_Wabble_Shotgun.obj");
	Flow::AssetSystem::LoadAsset("Wabble_Rifle_Collision", "Flow/Assets/Collision/Collision_Wabble_Rifle.obj");
	Flow::AssetSystem::LoadAsset("Wabble_Revolver_Collision", "Flow/Assets/Collision/Collision_Wabble_Revolver.obj");
	Flow::AssetSystem::LoadAsset("Wabble_Blunderbuss_Collision", "Flow/Assets/Collision/Collision_Wabble_Blunderbuss.obj");
	Flow::AssetSystem::LoadAsset("Wabble_Crate_Collision", "Flow/Assets/Collision/Collision_Wabble_Crate.obj");


	//Assign Collisions to meshes

	Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Table")->GetMesh(0)->CollisionName_ = "Wabble_Table_Collision";
	Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Chair")->GetMesh(0)->CollisionName_ = "Wabble_Chair_Collision";
	Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Shotgun")->GetMesh(0)->CollisionName_ = "Wabble_Shotgun_Collision";
	Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Rifle")->GetMesh(0)->CollisionName_ = "Wabble_Rifle_Collision";
	Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Revolver")->GetMesh(0)->CollisionName_ = "Wabble_Revolver_Collision";
	Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Blunderbuss")->GetMesh(0)->CollisionName_ = "Wabble_Blunderbuss_Collision";
	Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Wabble_Crate")->GetMesh(0)->CollisionName_ = "Wabble_Crate_Collision";

	//= Textures =
	Flow::AssetSystem::LoadAsset("ExampleRed", "Flow/Assets/Textures/ExampleRed.png");
	Flow::AssetSystem::LoadAsset("TestTexture", "Flow/Assets/Textures/TestTexture.png");
	Flow::AssetSystem::LoadAsset("TestTextureFlip", "Flow/Assets/Textures/TestTextureFlip.png");
	Flow::AssetSystem::LoadAsset("CharacterTexture", "Flow/Assets/Textures/CharacterTexture.png"); 
	Flow::AssetSystem::LoadAsset("Wabble_Props", "Flow/Assets/Textures/Wabble_Props.png"); 
	Flow::AssetSystem::LoadAsset("Wabble_Weapons", "Flow/Assets/Textures/Wabble_Weapons.png"); 
	Flow::AssetSystem::LoadAsset("Wabble_Wood", "Flow/Assets/Textures/Wabble_Wood.png"); 
	Flow::AssetSystem::LoadAsset("Wabble_Sand", "Flow/Assets/Textures/Wabble_Sand.png"); 
	Flow::AssetSystem::LoadAsset("TrainTex", "Flow/Assets/Textures/TrainTexture.png"); 


	Flow::AssetSystem::CreateMaterial<Flow::Mat_TexturedPhong>("Mat_Wabble_Props");
	Flow::Mat_TexturedPhong* PropsMat = static_cast<Flow::Mat_TexturedPhong*>(Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wabble_Props")->GetMaterial());
	PropsMat->SetTexture("Wabble_Props");
	PropsMat->SetPixelShader("TexturedPhongPS");
	PropsMat->SetVertexShader("TexturedPhongVS");
	
	Flow::AssetSystem::CreateMaterial<Flow::Mat_TexturedPhong>("Mat_Wabble_Weapons");
	Flow::Mat_TexturedPhong* WeaponsMat = static_cast<Flow::Mat_TexturedPhong*>(Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wabble_Weapons")->GetMaterial());
	WeaponsMat->SetTexture("Wabble_Weapons");
	WeaponsMat->SetPixelShader("TexturedPhongPS");
	WeaponsMat->SetVertexShader("TexturedPhongVS");
	
	Flow::AssetSystem::CreateMaterial<Flow::Mat_TexturedPhong>("Mat_Wood");
	Flow::Mat_TexturedPhong* WoodMat = static_cast<Flow::Mat_TexturedPhong*>(Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wood")->GetMaterial());
	WoodMat->SetTexture("Wabble_Wood");
	WoodMat->SetPixelShader("TexturedPhongPS");
	WoodMat->SetVertexShader("TexturedPhongVS");
	
	Flow::AssetSystem::CreateMaterial<Flow::Mat_TexturedPhong>("Mat_Sand");
	Flow::Mat_TexturedPhong* SandMat = static_cast<Flow::Mat_TexturedPhong*>(Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Sand")->GetMaterial());
	SandMat->SetTexture("Wabble_Sand");
	SandMat->SetPixelShader("TexturedPhongPS");
	SandMat->SetVertexShader("TexturedPhongVS");

	Flow::MeshAsset* PlaneMesh = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("CharacterPlane");
	Flow::AssetSystem::CreateMaterial<Flow::Mat_TexturedPhong>("Mat_CharacterPlane");
}
