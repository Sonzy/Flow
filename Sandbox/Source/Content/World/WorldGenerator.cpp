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
	//Flow::MaterialAsset* LitGrey = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_LitGrey");
	Flow::MaterialAsset* LitGrey = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wood");

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
	: WallSpeed_(100.0f), EndZ_(-750.0f), WallInterval_(5.0f)
{
	Flow::MeshAsset* Cube = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Box");
	//Flow::MaterialAsset* CubeMat = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_LitGrey");
	Flow::MaterialAsset* CubeMat = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wood");

	Base = Flow::World::GetWorld()->SpawnWorldObject<MeshWorldObject>("Base");
	LeftWall = Flow::World::GetWorld()->SpawnWorldObject<MeshWorldObject>("Left");
	RightWall = Flow::World::GetWorld()->SpawnWorldObject<MeshWorldObject>("Right");
	Top = Flow::World::GetWorld()->SpawnWorldObject<MeshWorldObject>("Top");

	Base->GetMeshComponent()->SetMeshAndMaterial(Cube, CubeMat->GetMaterial());
	LeftWall->GetMeshComponent()->SetMeshAndMaterial(Cube, CubeMat->GetMaterial());
	RightWall->GetMeshComponent()->SetMeshAndMaterial(Cube, CubeMat->GetMaterial());
	Top->GetMeshComponent()->SetMeshAndMaterial(Cube, CubeMat->GetMaterial());

	Base->SetPhysicsMode(Flow::PhysicsMode::Static);
	LeftWall->SetPhysicsMode(Flow::PhysicsMode::Static);
	RightWall->SetPhysicsMode(Flow::PhysicsMode::Static);
	Top->SetPhysicsMode(Flow::PhysicsMode::Static);

	Base->GetRootComponent()->SetRelativeScale(Vector(500.0f, 20.0f, 2500.0f));
	LeftWall->GetRootComponent()->SetRelativeScale(Vector(500.0f, 20.0f, 2500.0f));
	RightWall->GetRootComponent()->SetRelativeScale(Vector(500.0f, 20.0f, 2500.0f));
	Top->GetRootComponent()->SetRelativeScale(Vector(500.0f, 20.0f, 2500.0f));

	Base->GetRootComponent()->SetRelativePosition(Vector(0.0f, -250.0f, 0.0f));
	LeftWall->GetRootComponent()->SetRelativePosition(Vector(-250.0f, 0.0f, 0.0f));
	RightWall->GetRootComponent()->SetRelativePosition(Vector(250.0f, 0.0f, 0.0f));
	Top->GetRootComponent()->SetRelativePosition(Vector(0.0f, 250.0f, 0.0f));

	Base->GetRootComponent()->SetRelativeRotation(Rotator(0.0f, 0.0f,0.0f));
	LeftWall->GetRootComponent()->SetRelativeRotation(Rotator(0.0f, 90.0f,0.0f));
	RightWall->GetRootComponent()->SetRelativeRotation(Rotator(0.0f, 90.0f,0.0f));
	Top->GetRootComponent()->SetRelativeRotation(Rotator(0.0f, 0.0f,0.0f));
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
		Wall->GetRootComponent()->AddRelativePosition(Vector(0.0f, 0.0f, -WallSpeed_ * DeltaTime));

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
		if (Walls_.erase(std::find(Walls_.begin(), Walls_.end(), Wall)) == Walls_.end())
			FLOW_ERROR("Failed to find wall to remove");
	}

	return SpawnWall;
}

void WorldGenerator::Render()
{
	for (auto& Wall : Walls_)
	{
		Wall->Render();
	}

	Base->Render();
	LeftWall->Render();
	RightWall->Render();
	Top->Render();
}

void WorldGenerator::CreateWall(Flow::World* World)
{
	ObstacleWall* NewWall = FreeWalls_.size() > 0 ? FreeWalls_[0] : World->SpawnWorldObjectDeferred<ObstacleWall>("Wall").get();
	if(FreeWalls_.size() > 0)
		FreeWalls_.pop_back();
	NewWall->Initialise(Vector(0.0f, 0.0f, 500.0f));
	NewWall->SetVisibility(true);

	!NewWall->BegunPlay_ ? NewWall->BeginPlay() : NewWall->GetRootComponent()->UpdatePhysicsBody();

	Walls_.push_back(NewWall);
}
