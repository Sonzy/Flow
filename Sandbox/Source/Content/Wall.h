#pragma once
#include "Flow\GameFramework\WorldObject.h"
#include "Flow\Helper\Maths.h"

namespace Flow
{
	class StaticMeshComponent;
}

/* Simple wall class with physics */
class Wall : public Flow::WorldObject
{
public:
	Wall();
	Wall(const std::string& NewName);
	~Wall() = default;

	virtual void Tick(float DeltaTime) override;

	virtual void InitialisePhysics() override;

	void UpdateScale(Vector Scale);

protected:

	std::shared_ptr<Flow::StaticMeshComponent> WallMesh;
};