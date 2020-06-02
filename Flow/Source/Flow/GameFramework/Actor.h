#pragma once
#include "Flow\Logging\Log.h"
#include "Gameobject.h"
#include "Components\Component.h"
#include <memory>
#include <type_traits>

#include "Flow\Helper\Maths.h"
#include "btBulletCollisionCommon.h"

class WorldComponent;
class Controller;

class FLOW_API Actor : public GameObject
{
public:
	Actor();
	Actor(const std::string& Name);
	virtual ~Actor();

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

	WorldComponent* GetRootComponent();

	Vector GetLocation();
	Vector GetScale();
	Rotator GetRotation();

	void Render();

	bool IsSimulatingPhysics();
	bool CollisionEnabled();

	virtual void InitialisePhysics();
	btRigidBody* GetRigidBody();

	virtual void DrawDetailsWindow(bool bDontUpdate) override;

	void SetVisibility(bool Visible);

protected:

	/* Creates a new component, assigns it's parent as this object and returns it */
	template <typename T>
	std::shared_ptr<T> CreateComponent(const std::string& NewName)
	{
		static_assert(std::is_base_of<Component, T>::value, "Tried to create a component templated with a non-component type");

		std::shared_ptr<T> NewComponent = std::make_shared<T>(NewName);
		Component* Comp = static_cast<Component*>(NewComponent.get());
		Comp->SetParent(this);
		return NewComponent;
	}

protected:

	WorldComponent* _RootComponent;

	//= Physics ===================

	bool _SimulatePhysics = false;
	bool _HasCollision = false;
	int  _Tag;
	bool _Visible = true;


	//= Controller

	Controller* _CurrentController;

};