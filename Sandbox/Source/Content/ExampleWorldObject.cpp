#include "ExampleWorldObject.h"
#include "Flow\Rendering\Core\Materials\Material.h"
#include "Flow\Assets\Materials\Mat_FlatColour.h"
#include "Flow\Assets\AssetSystem.h"

#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Assets\Materials\MaterialAsset.h"

ExampleWorldObject::ExampleWorldObject()
	: ExampleWorldObject("Example World Object")
{

}

ExampleWorldObject::ExampleWorldObject(const std::string& NewName)
	: WorldObject(NewName)
{
	TestMesh = CreateComponent<Flow::StaticMeshComponent>("Test Mesh");
	TestMesh2 = CreateComponent<Flow::StaticMeshComponent>("Test Mesh 2");
	m_RootComponent = TestMesh.get();

	m_RootComponent->AddChild(TestMesh2.get());

	Flow::MeshAsset* Mesh = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Hat_Sherif");
	Flow::MeshAsset* Mesh2 = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Hat_FancyMan");
	Flow::Material* Material = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_HatFancyMan")->GetMaterial();
	TestMesh->InitialiseComponent(Mesh2, Material);
	TestMesh2->InitialiseComponent(Mesh, Material);
	TestMesh2->SetRelativeLocation(Vector(100.0f, 10.0f, 10.0f));
	//TestMesh2->SetWorldRotation(Rotator(90.0f, 0.0f, 0.0f));
}

void ExampleWorldObject::Tick(float DeltaTime)
{
	//TestMesh2->SetRelativeLocation(TestMesh2->GetRelativeLocation() + Vector(1.0f, 0.0f, 0.0f));
	TestMesh2->SetRelativeRotation(TestMesh2->GetRelativeRotation() + Rotator(1.0f * DeltaTime, 0.0f, 0.0f));

	//FLOW_LOG("TestMeshPosition: {0}", TestMesh->GetRelativeLocation());
}

