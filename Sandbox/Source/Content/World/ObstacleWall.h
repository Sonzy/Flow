#pragma once
#include "Flow\GameFramework\WorldObject.h"
#include <random>

namespace Flow
{
	class StaticMeshComponent;
}

struct WallLayout
{
	WallLayout(float GapWidth, float GapHeight, float CubeWidth, int HorizontalGaps)
		: GapWidth_(GapWidth), CubeWidth_(GapHeight), GapHeight_(CubeWidth), HorizontalGaps_(HorizontalGaps)
	{

	}

	WallLayout()
		: GapWidth_(100), CubeWidth_(100), GapHeight_(100), HorizontalGaps_(5)
	{

	}

	float GapWidth_;
	float CubeWidth_;
	float GapHeight_;

	int HorizontalGaps_ = 5;
};

struct WallLayout2
{
	WallLayout2(Vector Pos, Vector Scale)
		: Position_(Pos), Scale_(Scale)
	{}
	Vector Position_;
	Vector Scale_;
};
struct WallLayoutContainer
{
	WallLayoutContainer(float Height, float HeightPosition = 0)
		: Height_(Height), HeightPos_(HeightPosition)
	{

	}

	void AddCube(WallLayout2 NewCube)
	{
		Cubes_.push_back(NewCube);
	}

	std::vector<WallLayout2> Cubes_;
	float Height_;
	float HeightPos_;

};

class ObstacleWall : public Flow::WorldObject
{
public:
	ObstacleWall();
	ObstacleWall(const std::string& Name);

	void Initialise(Vector StartPosition);

	void CreateWall(WallLayout NewLayout);
	void CreateWall(WallLayoutContainer NewLayout);
	void CreateWall();

protected:
	std::shared_ptr<Flow::WorldComponent> EmptyRoot_;
	std::shared_ptr<Flow::StaticMeshComponent> Cube_Bottom_;
	std::shared_ptr<Flow::StaticMeshComponent> Cube_Top_;

	WallLayout Layout_;

	std::vector<std::shared_ptr<Flow::StaticMeshComponent>> Boxes_;
	std::vector<WallLayoutContainer> Layouts_;
	std::mt19937 Random_;
	std::uniform_int_distribution<int> Distribution_;
};