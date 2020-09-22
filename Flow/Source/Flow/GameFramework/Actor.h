#pragma once
#include "Flow\Logging\Log.h"
#include "Gameobject.h"
#include "Components\Component.h"
#include <memory>
#include <type_traits>
#include <fstream>

#include "Maths/Maths.h"
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

#if WITH_EDITOR
	virtual void EditorBeginPlay() override;
	virtual void OnViewportSelected() override;
	virtual void OnViewportDeselected() override;
#endif
	virtual void Tick(float DeltaTime) override;

	WorldComponent* GetRootComponent() const;

	Vector3 GetLocation();
	Vector3 GetScale();
	Rotator GetRotation();

	void Render();

	bool IsSimulatingPhysics();
	bool CollisionEnabled();

	virtual void DrawDetailsWindow(bool bDontUpdate) override;

	void SetVisibility(bool Visible);

	bool IsTickEnabled() { return _TickEnabled; }

	Component* GetComponentByName(const std::string& Name) const;

public:

	virtual void Serialize(std::ofstream* Archive);
	void SerializeComponents(std::ofstream* Archive);
	virtual void Deserialize(std::ifstream* Archive);
	void DeserializeComponents(std::ifstream* Archive);

	friend std::ofstream& operator<<(std::ofstream& Out, const Actor& Object)
	{
		//Name of actor (TODO: Max character length)
		const std::string& Name = Object.GetName();
		

		Out.write(Name.c_str(), sizeof(char) * 32);
		std::streampos Pos = Out.tellp();
		FLOW_ENGINE_LOG("Offset: {0}", static_cast<int>(Pos));

		Out << Object.GetRootComponent();
	}

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

	int  _Tag;
	bool _Visible = true;

	//= Controller
	Controller* _CurrentController;

protected:
	friend class Level;

	/* This only works in the constructor of a spawned actor, */
	bool _TickEnabled = true;
};

