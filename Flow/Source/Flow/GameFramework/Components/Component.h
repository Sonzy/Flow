#pragma once
#include "Flow\GameFramework\Gameobject.h"
#include <vector>

class Actor;
class WorldComponent;

/* Base class for all components, contains attachment properties to a world object */
class FLOW_API Component : public GameObject
{
public:
	Component();
	Component(const std::string& Name);
	virtual ~Component();

	WorldComponent* GetParentComponent() const;
	Actor* GetParentActor() const;

	void SetParent(Actor* Parent);
	void SetParentComponent(WorldComponent* Parent);

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

	virtual void DrawComponentDetailsWindow();

protected:

	Actor* _ParentObject;
	WorldComponent* _AttachedComponent;
};