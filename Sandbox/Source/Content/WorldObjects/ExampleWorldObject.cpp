#include "ExampleWorldObject.h"
#include "Flow\Rendering\Core\Materials\Material.h"
#include "Flow\Assets\Materials\Mat_FlatColour.h"
#include "Flow\Assets\AssetSystem.h"

#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Assets\Materials\MaterialAsset.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

ExampleWorldObject::ExampleWorldObject()
	: ExampleWorldObject("Example World Object")
{
	_SimulatePhysics = true;
	_HasCollision = true;
}

ExampleWorldObject::ExampleWorldObject(const std::string& NewName)
	: Actor(NewName)
{
	TestMesh = CreateComponent<StaticMeshComponent>("Test Mesh");
	TestMesh2 = CreateComponent<StaticMeshComponent>("Test Mesh 2");
	_RootComponent = TestMesh.get();

	_RootComponent->AddChild(TestMesh2.get());

	//MeshAsset* Mesh = AssetSystem::GetAsset<MeshAsset>("Hat_Sherif");
	MeshAsset* Mesh = AssetSystem::GetAsset<MeshAsset>("SelectionGizmo");
	MeshAsset* Mesh2 = AssetSystem::GetAsset<MeshAsset>("Hat_FancyMan");
	Material* Material = AssetSystem::GetAsset<MaterialAsset>("Mat_HatFancyMan")->GetMaterial();
	TestMesh->InitialiseComponent(Mesh2, Material);
	TestMesh2->InitialiseComponent(Mesh, Material);
	TestMesh2->SetRelativePosition(Vector(100.0f, 10.0f, 10.0f));
	//TestMesh2->SetWorldRotation(Rotator(90.0f, 0.0f, 0.0f));

	TestMesh->SetSimulatePhysics(true);
}

void ExampleWorldObject::Tick(float DeltaTime)
{
	//TestMesh2->SetRelativeLocation(TestMesh2->GetRelativeLocation() + Vector(1.0f, 0.0f, 0.0f));
	TestMesh2->SetRelativeRotation(TestMesh2->GetRelativeRotation() + Rotator(1.0f * DeltaTime, 0.0f, 0.0f));

	TestMesh->Tick(DeltaTime);

	//FLOW_LOG("TestMeshPosition: {0}", TestMesh->GetRelativeLocation());
}

void ExampleWorldObject::InitialisePhysics()
{
	Actor::InitialisePhysics();
}
