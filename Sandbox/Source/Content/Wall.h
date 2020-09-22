#pragma once
#include "Flow\GameFramework\Actor.h"

class StaticMeshComponent;

/* Simple wall class with physics */
class Wall : public Actor
{
public:
	Wall();
	Wall(const std::string& NewName);
	~Wall() = default;

	virtual void Tick(float DeltaTime) override;


	void UpdateScale(Vector3 Scale);

protected:

	std::shared_ptr<StaticMeshComponent> WallMesh;
};