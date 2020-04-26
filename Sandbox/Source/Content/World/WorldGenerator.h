#pragma once
#include <vector>
#include <memory>
#include "Flow\GameFramework\WorldObject.h"
#include "Content\WorldObjects\MeshWorldObject.h"
#include "Flow\Helper\Timer.h"

class ObstacleWall;

namespace Flow
{
	class World;
}

class WorldGenerator
{
public:
	static std::vector<std::shared_ptr<MeshWorldObject>> CreateWorld(IntVector2D Worldresolution, float BuildingGap, int BuildingBlockSize, float RoadGap, Vector Scale);
	static std::vector<std::shared_ptr<MeshWorldObject>> CreateObstacle();


	//= Proper stoof ======

	WorldGenerator();
	~WorldGenerator();

	bool Update(float DeltaTime);
	void Render();

	void CreateWall(Flow::World* World);

protected:

	std::vector<ObstacleWall*> Walls_;
	std::vector<ObstacleWall*> FreeWalls_;
	Flow::Timer WallTimer;
	float WallSpeed_;
	float EndZ_;
	float WallInterval_;
};