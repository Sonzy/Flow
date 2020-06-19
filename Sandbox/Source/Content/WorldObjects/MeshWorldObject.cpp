#include "Flowpch.h"
#include "MeshWorldObject.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"

#include "Flow\Assets\AssetSystem.h"

#include "Flow\Assets\Materials\MaterialAsset.h"
#include "Flow\Assets\Meshes\MeshAsset.h"

MeshWorldObject::MeshWorldObject()
	: Actor("Mesh Object")
{
}

//TODO: Get rid of this lul
MeshWorldObject::MeshWorldObject(const std::string& NewName)
	: Actor(NewName)
{
	//_SimulatePhysics = true;
	//_HasCollision = true;

	//Initialise Components
	MeshComponent = CreateComponent<StaticMeshComponent>("Mesh Component");
	_RootComponent = MeshComponent.get();

	//Initialise Mesh Component
	// Flow::MeshAsset* Mesh = Flow::AssetSystem::GetAsset<Flow::MeshAsset>(MeshName);
	// Flow::Material* Material = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>(MaterialName)->GetMaterial();
	// MeshComponent->InitialiseComponent(Mesh, Material);
	// MeshComponent->SetSimulatePhysics(SimulatePhysics);
	// MeshComponent->InitialisePhysics();
}

MeshWorldObject::MeshWorldObject(const std::string& NewName, const std::string& MeshName, const std::string& MaterialName)
	: Actor(NewName), MeshName(MeshName), MaterialName(MaterialName)
{
	//_SimulatePhysics = true;
	//_HasCollision = true;

	//Initialise Components
	MeshComponent = CreateComponent<StaticMeshComponent>("Mesh Component");
	_RootComponent = MeshComponent.get();

	//Initialise Mesh Component
	MeshAsset* Mesh = AssetSystem::GetAsset<MeshAsset>(MeshName);
	MaterialAsset* Material = AssetSystem::GetAsset<MaterialAsset>(MaterialName);
	MeshComponent->SetMeshAndMaterial(Mesh, Material);
	//MeshComponent->SetSimulatePhysics(_SimulatePhysics);
	//MeshComponent->InitialisePhysics();
}

StaticMeshComponent* MeshWorldObject::GetMeshComponent() const
{
	return MeshComponent.get();
}

void MeshWorldObject::InitPhysics(bool Simulate)
{
	MeshComponent->SetSimulatePhysics(Simulate);
	MeshComponent->InitialisePhysics();
}
