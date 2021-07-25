#pragma once

// Includes ////////////////////////////////////////////////////////////

#include <fstream>
#include <memory>
#include <type_traits>

#include "Gameobject.h"
#include "Bullet/btBulletCollisionCommon.h"
#include "Components/Component.h"
#include "Components/Component.h"
#include "Framework/Logging\Log.h"
#include "GameFramework/World.h"
#include "Maths/Maths.h"

// Forward Declarations ////////////////////////////////////////////////

class WorldComponent;
class Controller;

// Class Definitions ///////////////////////////////////////////////////

class FLOW_API Actor : public GameObject
{
	friend class Level;

public:

	REGISTER_CLASS(Actor)

	// Public Functions ////////////////////////////////////////////////

							Actor();
							Actor(const std::string& Name);
	virtual					~Actor();

	virtual void			OnRegistered();
	virtual void			BeginPlay();
	virtual void			Tick(float DeltaTime) override;
	void					Render();

#if WITH_EDITOR
	virtual void			EditorBeginPlay() override;
	virtual void			OnViewportSelected() override;
	virtual void			OnViewportDeselected() override;
#endif


	WorldComponent*			SetRootComponent(WorldComponent* NewRoot);
	WorldComponent*			GetRootComponent() const;
	Vector3					GetLocation() const;
	Vector3					GetScale() const;
	Rotator					GetRotation() const;
	Transform				GetWorldTransform() const;
	void					SetWorldTransform(const Transform& transform);


	bool					IsSimulatingPhysics();
	bool					CollisionEnabled();
	void					DestroyPhysics();

	virtual void			DrawDetailsWindow(bool bDontUpdate) override;

	void					SetVisibility(bool Visible);
	bool					IsTickEnabled() const;

	std::vector<WorldComponent*> GetComponents() const;
	Component*				GetComponentByName(const std::string& Name) const;

	//= Save/Load =

	virtual void			Serialize(YAML::Emitter& Archive) override;
	virtual void			Deserialize(YAML::Node& Archive) override;

	friend std::ofstream& operator<<(std::ofstream& Out, const Actor& Object)
	{
		//Name of actor (TODO: Max character length)
		const std::string& Name = Object.GetName();
		

		Out.write(Name.c_str(), sizeof(char) * 32);
		std::streampos Pos = Out.tellp();
		FLOW_ENGINE_LOG("Offset: %d", static_cast<int>(Pos));

		Out << Object.GetRootComponent();
	}

	void DrawInspectionTree(WorldComponent* CurrentInspectedComponent, bool DontOpenTree = false);

protected:

	// Protected Functions /////////////////////////////////////////////

	/* Creates a new component, assigns it's parent as this object and returns it */
	template <typename T>
	T* CreateComponent(const std::string& NewName);

protected:

	// Protected Variables /////////////////////////////////////////////

	bool					m_tickEnabled;
	WorldComponent*			m_rootComponent;
	bool					m_visible;
	Controller*				m_currentController;

};

// Inline Function Definitions ////////////////////////////////////////////////

template <typename T>
inline T* Actor::CreateComponent(const std::string& NewName)
{
	static_assert(std::is_base_of<Component, T>::value, "Tried to create a component templated with a non-component type");

	T* NewComponent = new T(NewName);
	NewComponent->SetParent(this);

	World::Get().RegisterGameObject(NewComponent);

	return NewComponent;
}

inline bool Actor::IsTickEnabled() const
{
	return m_tickEnabled;
}