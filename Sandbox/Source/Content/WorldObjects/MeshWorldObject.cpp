#include "Flowpch.h"
#include "MeshWorldObject.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"

#include "Flow\Assets\AssetSystem.h"

#include "Flow\Assets\Materials\MaterialAsset.h"
#include "Flow\Assets\Meshes\MeshAsset.h"

MeshWorldObject::MeshWorldObject()
	: Flow::WorldObject("Mesh Object")
{
}

//TODO: Get rid of this lul
MeshWorldObject::MeshWorldObject(const std::string& NewName)
	: Flow::WorldObject(NewName)
{
	SimulatePhysics = true;
	HasCollision = true;

	//Initialise Components
	MeshComponent = CreateComponent<Flow::StaticMeshComponent>("Mesh Component");
	m_RootComponent = MeshComponent.get();

	//Initialise Mesh Component
	// Flow::MeshAsset* Mesh = Flow::AssetSystem::GetAsset<Flow::MeshAsset>(MeshName);
	// Flow::Material* Material = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>(MaterialName)->GetMaterial();
	// MeshComponent->InitialiseComponent(Mesh, Material);
	// MeshComponent->SetSimulatePhysics(SimulatePhysics);
	// MeshComponent->InitialisePhysics();
}

MeshWorldObject::MeshWorldObject(const std::string& NewName, const std::string& MeshName, const std::string& MaterialName)
	: Flow::WorldObject(NewName), MeshName(MeshName), MaterialName(MaterialName)
{
	SimulatePhysics = true;
	HasCollision = true;

	//Initialise Components
	MeshComponent = CreateComponent<Flow::StaticMeshComponent>("Mesh Component");
	m_RootComponent = MeshComponent.get();

	//Initialise Mesh Component
	Flow::MeshAsset* Mesh = Flow::AssetSystem::GetAsset<Flow::MeshAsset>(MeshName);
	Flow::Material* Material = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>(MaterialName)->GetMaterial();
	MeshComponent->InitialiseComponent(Mesh, Material);
	MeshComponent->SetSimulatePhysics(SimulatePhysics);
	//MeshComponent->InitialisePhysics();
}

Flow::StaticMeshComponent* MeshWorldObject::GetMeshComponent() const
{
	return MeshComponent.get();
}

void MeshWorldObject::InitPhysics(bool Simulate)
{
	MeshComponent->SetSimulatePhysics(Simulate);

	MeshComponent->InitialisePhysics();
}
