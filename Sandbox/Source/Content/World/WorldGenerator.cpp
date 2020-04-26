#include "WorldGenerator.h"
#include "Flow\GameFramework\World.h"
#include "Flow\Application.h"
#include "Content\WorldObjects\MeshWorldObject.h"
#include "Flow\GameFramework\Components\WorldComponent.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Materials\MaterialAsset.h"
#include "Flow\Assets\Meshes\MeshAsset.h"

#include "ObstacleWall.h"

std::vector<std::shared_ptr<MeshWorldObject>> WorldGenerator::CreateWorld(IntVector2D Worldresolution, float BuildingGap, int BuildingBlockSize, float RoadGap, Vector Scale)
{
	Flow::MeshAsset* Box = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Box");
	Flow::MaterialAsset* LitGrey = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_LitGrey");

	std::vector<std::shared_ptr<MeshWorldObject>> Buildings;
	Buildings.reserve(Worldresolution.X * Worldresolution.Y);

	char TextBuffer[64];
	float YPos = Scale.Y / 2;
	float DoubleX = Scale.X * 5;
	float DoubleZ = Scale.Z * 5;
	float SpacingPadding = 200.0f;

	for (int i = 0; i < Worldresolution.X; i++)
	{
		for (int j = 0; j < Worldresolution.Y; j++)
		{
			sprintf(TextBuffer, "Building %d", (i * Worldresolution.Y) + j);
			std::shared_ptr<MeshWorldObject> NewBuilding = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>(TextBuffer, false);

			static_cast<Flow::StaticMeshComponent*>(NewBuilding->GetRootComponent())->SetMeshAndMaterial(Box, LitGrey->GetMaterial());
			NewBuilding->SetPhysicsMode(Flow::PhysicsMode::Static);

			NewBuilding->GetRootComponent()->SetWorldPosition(Vector(i * (DoubleX + SpacingPadding), YPos, j * (DoubleZ + SpacingPadding)));
			NewBuilding->GetRootComponent()->SetWorldScale(Scale);

			Buildings.push_back(NewBuilding);
		}
	}

	return Buildings;
}

std::vector<std::shared_ptr<MeshWorldObject>> WorldGenerator::CreateObstacle()
{
	std::vector<std::shared_ptr<MeshWorldObject>> OutObjects;

	///char TextBuffer[64];

	//One on top and bottom
	//std::shared_ptr<MeshWorldObject> Top = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>(TextBuffer, false);
	//std::shared_ptr<MeshWorldObject> Bottom = Flow::Application::GetWorld()->SpawnWorldObject<MeshWorldObject>(TextBuffer, false);
	return OutObjects;
}

WorldGenerator::WorldGenerator()
	: WallSpeed_(1.0f), EndZ_(-500.0f), WallInterval_(5.0f)
{

}

WorldGenerator::~WorldGenerator()
{
	Walls_.clear();
	FreeWalls_.clear();
}

bool WorldGenerator::Update(float DeltaTime)
{
	bool SpawnWall = false;
	if (WallTimer.Peek() > WallInterval_)
	{
		WallTimer.Mark();
		SpawnWall = true;
	}

	std::vector<ObstacleWall*> ToRemove;
	for (auto& Wall : Walls_)
	{
		Wall->GetRootComponent()->AddRelativePosition(Vector(0.0f, 0.0f, -WallSpeed_));

		if (Wall->GetRootComponent()->GetRelativePosition().Z < EndZ_)
		{
			Wall->SetVisibility(false);
			ToRemove.push_back(Wall);
			FreeWalls_.push_back(Wall);
		}

		Wall->GetRootComponent()->UpdatePhysicsBody(true);
	}

	for (auto& Wall : ToRemove)
	{
		Walls_.erase(std::find(Walls_.begin(), Walls_.end(), Wall));
	}

	return SpawnWall;
}

void WorldGenerator::Render()
{
	for (auto& Wall : Walls_)
	{
		Wall->Render();
	}
}

void WorldGenerator::CreateWall(Flow::World* World)
{
	ObstacleWall* NewWall = FreeWalls_.size() > 0 ? FreeWalls_[0] : World->SpawnWorldObject<ObstacleWall>("Wall").get();
	if(FreeWalls_.size() > 0)
		FreeWalls_.pop_back();
	NewWall->Initialise(Vector(0.0f, 0.0f, 500.0f));
	NewWall->SetVisibility(true);
	Walls_.push_back(NewWall);
}
