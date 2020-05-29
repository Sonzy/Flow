#pragma once
#include "Flow\GameFramework\WorldObject.h"
#include "Flow\Helper\Maths.h"

class StaticMeshComponent;

/* Simple wall class with physics */
class Wall : public WorldObject
{
public:
	Wall();
	Wall(const std::string& NewName);
	~Wall() = default;

	virtual void Tick(float DeltaTime) override;

	virtual void InitialisePhysics() override;

	void UpdateScale(Vector Scale);

protected:

	std::shared_ptr<StaticMeshComponent> WallMesh;
};