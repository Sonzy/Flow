#include "Wall.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"

#include "Flow\Rendering\Core\Materials\Material.h"
#include "Flow\Assets\Materials\Mat_FlatColour.h"
#include "Flow\Assets\AssetSystem.h"

#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Assets\Materials\MaterialAsset.h"

#include "Flow\GameFramework\World.h"

Wall::Wall()
	: Wall("Wall")
{

}

Wall::Wall(const std::string& NewName)
	: Actor(NewName)
{
	MeshAsset* Mesh = AssetSystem::GetAsset<MeshAsset>("Box");
	Material* Material = AssetSystem::GetAsset<MaterialAsset>("Mat_Wood")->GetMaterial();

	WallMesh = CreateComponent<StaticMeshComponent>("Test Mesh");
	WallMesh->InitialiseComponent(Mesh, Material);
	_RootComponent = WallMesh.get();

	_HasCollision = true;
}

void Wall::Tick(float DeltaTime)
{
}

void Wall::InitialisePhysics()
{
	Actor::InitialisePhysics();

	//WallMesh->InitialisePhysics();

	//WallMesh->MovePhysicsBody(WallMesh->GetRelativeTransform());
}

void Wall::UpdateScale(Vector Scale)
{
	Transform WallTrans = WallMesh->GetRelativeTransform();
	WallMesh->MovePhysicsBody(WallTrans);
}
