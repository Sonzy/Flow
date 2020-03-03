#include "ExampleWorldObject.h"
#include "Flow\Rendering\Core\Materials\Material.h"
#include "Flow\Assets\Materials\Mat_FlatColour.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Materials\MaterialAsset.h"

ExampleWorldObject::ExampleWorldObject()
	: WorldObject("Example World Object")
{
	TestMesh = CreateComponent<Flow::StaticMeshComponent>("Test Mesh");
	m_RootComponent = TestMesh.get();

	TestMesh->InitialiseComponent("Hat_FancyMan", Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_HatFancyMan")->GetMaterial());
}

ExampleWorldObject::ExampleWorldObject(const std::string& NewName)
	: WorldObject(NewName)
{
	TestMesh = CreateComponent<Flow::StaticMeshComponent>("Test Mesh");
	m_RootComponent = TestMesh.get();

	TestMesh->InitialiseComponent("Hat_FancyMan", Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_HatFancyMan")->GetMaterial());

}
