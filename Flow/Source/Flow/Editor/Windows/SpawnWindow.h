#pragma once

class World;

class SpawnWindow
{
public:

	SpawnWindow(World* WorldRef);

	void Draw();

private:

	World* _WorldReference;
	float _SpawnDistance;
};