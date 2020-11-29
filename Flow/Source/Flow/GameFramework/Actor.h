#pragma once

//= Includes ===============================================

#include <fstream>
#include <memory>
#include <type_traits>

#include "Bullet/btBulletCollisionCommon.h"
#include "Components\Component.h"
#include "Components\Component.h"
#include "Flow\Logging\Log.h"
#include "Gameobject.h"
#include "Maths/Maths.h"

//= Forward Declarations ===================================

class WorldComponent;
class Controller;

//= Class Definitions ======================================

class FLOW_API Actor : public GameObject
{
public:

	//= Public Functions ============================

							Actor();
							Actor(const std::string& Name);
	virtual					~Actor();

	virtual void			BeginPlay();

#if WITH_EDITOR
	virtual void			EditorBeginPlay() override;
	virtual void			OnViewportSelected() override;
	virtual void			OnViewportDeselected() override;
#endif
	virtual void			Tick(float DeltaTime) override;

	WorldComponent*			SetRootComponent(WorldComponent* NewRoot);

	WorldComponent*			GetRootComponent() const;
	Vector3					GetLocation();
	Vector3					GetScale();
	Rotator					GetRotation();

	void					Render();

	bool					IsSimulatingPhysics();
	bool					CollisionEnabled();
	void					DestroyPhysics();

	virtual void			DrawDetailsWindow(bool bDontUpdate) override;

	void					SetVisibility(bool Visible);
	bool					IsTickEnabled() { return m_TickEnabled; }

	std::vector<WorldComponent*> GetComponents() const;
	Component*				GetComponentByName(const std::string& Name) const;

	virtual void			Serialize(std::ofstream* Archive);
	void					SerializeComponents(std::ofstream* Archive);
	virtual void			Deserialize(std::ifstream* Archive);
	void					DeserializeComponents(std::ifstream* Archive);

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

	//= Protected Template Functions =======================

	/* Creates a new component, assigns it's parent as this object and returns it */
	template <typename T>
	T* CreateComponent(const std::string& NewName)
	{
		static_assert(std::is_base_of<Component, T>::value, "Tried to create a component templated with a non-component type");

		T* NewComponent = new T(NewName);
		Component* Comp = static_cast<Component*>(NewComponent);
		Comp->SetParent(this);
		return NewComponent;
	}

protected:
	friend class Level;

	//= Protected Variables ===============================

	/* This only works in the constructor of a spawned actor, */
	bool					m_TickEnabled = true;
	WorldComponent*			m_RootComponent;
	int						m_Tag;
	bool					m_Visible = true;
	Controller*				m_CurrentController;

};

