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
	MaterialAsset* Material = AssetSystem::GetAsset<MaterialAsset>("Mat_Wood");

	WallMesh = CreateComponent<StaticMeshComponent>("Test Mesh");
	WallMesh->SetMeshAndMaterial(Mesh, Material);
	m_RootComponent = WallMesh.get();

//	_HasCollision = true;
}

void Wall::Tick(float DeltaTime)
{
}

//void Wall::InitialisePhysics()
//{
//	Actor::InitialisePhysics();
//
//	//WallMesh->InitialisePhysics();
//
//	//WallMesh->MovePhysicsBody(WallMesh->GetRelativeTransform());
//}

void Wall::UpdateScale(Vector3 Scale)
{
	Transform WallTrans = WallMesh->GetRelativeTransform();
	//WallMesh->MovePhysicsBody(WallTrans);
}
