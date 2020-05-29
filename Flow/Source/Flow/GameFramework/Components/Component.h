#pragma once
#include "Flow\GameFramework\Gameobject.h"
#include <vector>

class WorldObject;
class WorldComponent;

/* Base class for all components, contains attachment properties to a world object */
class FLOW_API Component : public GameObject
{
public:
	Component();
	Component(const std::string& Name);
	virtual ~Component();

	WorldComponent* GetParentComponent() const;
	WorldObject* GetParentWorldObject() const;

	void SetParent(WorldObject* Parent);
	void SetParentComponent(WorldComponent* Parent);

	virtual void Tick(float DeltaTime) override;

	virtual void DrawComponentDetailsWindow();

protected:

	WorldObject* _ParentObject;
	WorldComponent* _AttachedComponent;
};