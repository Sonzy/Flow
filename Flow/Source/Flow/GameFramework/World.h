#pragma once

//= Includes ============================================

#include <vector>
#include <memory>
#include "Bullet/btBulletDynamicsCommon.h"
#include "Core.h"
#include "GameFramework/Level.h"
#include "Rendering/Core/DebugDrawing/LineBatcher.h"
#include "Maths/Vector3.h"
#include "Utils\BulletDebugDrawing.h"
#include "Typedefs.h"
#include "Utils/ComponentHelper.h"

//= Forward Declarations =================================

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class Actor;
class Skybox;
class Controller;
class Component;
class GameObject;

//= Enum Definitions ====================================

enum class WorldState
{
	Paused,
	Editor,
	InGame
};

//= Class Definition ====================================

class FLOW_API World
{
public:

	//= Public Template Functions ===================

	template<typename T>
	T* SpawnActor(const std::string& Name)
	{
		static_assert(std::is_base_of<Actor, T>::value, "Trying to spawn a class that isn't an actor");

		T* newActor = new T(Name);
		RegisterGameObject(newActor);

		return newActor;
	}

	template<typename T>
	T* DestroyActor(T* act)
	{
		static_assert(std::is_base_of<Actor, T>::value, "Tried to destroy a component templated with a non-component type");

		return DestroyActor(act->GetGuid()) ? nullptr : act;
	}

	template<typename T>
	T* DestroyComponent(T* comp)
	{
		static_assert(std::is_base_of<Component, T>::value, "Tried to destroy a component templated with a non-component type");

		return DestroyComponent(comp->GetGuid()) ? nullptr : comp;
	}


	//= Public Functions ============================

														World();
														World(const std::string& WorldName);
														~World();

	void												Tick(float DeltaTime);
	void												SaveLevel();
	void												LoadLevel();
	void												SavePlayState();
	void												LoadPlayState();

	//TODO: Temp rendering in the level
	void												Render();

	void												InitialiseWorld();
#if WITH_EDITOR
	void												StartEditor();
#endif

	//= Actor manipulation = 

	bool												DestroyActor(FGUID guid);
	bool												DestroyComponent(FGUID guid);

	//= Object Management =
	Actor*												FindActor(FGUID guid) const;
	Component*											FindComponent(FGUID guid) const;

	template<typename T>
	T* FindActor(FGUID guid) const
	{
		return dynamic_cast<T*>(FindActor(guid));
	}

	template<typename T>
	T* FindComponent(FGUID guid) const
	{
		return dynamic_cast<T*>(FindComponent(guid));
	}

	const std::string&									GetName();

	static btCollisionWorld::ClosestRayResultCallback	WorldTrace(Vector3 Start, Vector3 End);

	static btDiscreteDynamicsWorld*						GetPhysicsWorld();
	static World*										Get();

	void												AddPhysicsObject(btRigidBody* Obj);
	void												AddCollisionObject(btCollisionObject* Obj);

	//= Controllers ========

	void												RegisterController(Controller* NewController);
	void												DeRegisterController(Controller* OldController);
	Controller*											GetLocalController() const;

	//= Registration =

	void												RegisterGameObject(GameObject* newObject);
	void												RegisterGameObject(GameObject* newObject, FGUID guid); //Register with specific guid


	//= Getters etc
	LineBatcher&										GetLineBatcher() const	{ return sm_LineBatcher; };
	static LineBatcher&									GetLineBatcher_S();
	BulletDebugDraw&									GetPhysicsDebugDrawer() { return m_DebugDrawer; }

	const std::unordered_map<FGUID, Actor*>&			GetActorMap()			{ return m_actorMap; }
	const std::unordered_map<FGUID, Component*>&		GetComponentMap()		{ return m_componentMap; }

	WorldState											GetWorldState() const	{ return m_WorldState; }

	void												PrintAllPhysicsObjects() const;
protected:
	friend class Application;

	//= Protected Functions ===============

	void												StartGame();
	void												PauseGame();
	void												StopGame();
	void												InitialisePhysics(bool Force = false);

private:
	friend class Inspector;

	//= Private Variables =================

	std::string											m_WorldName;
	Level*												m_MainLevel;
	WorldState											m_WorldState;

	//= Lookup =

	std::unordered_map<FGUID, Actor*>					m_actorMap;
	std::unordered_map<FGUID, Component*>				m_componentMap;

	//=== World Physics ===

	/* Default memory setup */
	btDefaultCollisionConfiguration*					m_CollisionConfig;
	/* Default single threaded collision dispatcher */
	btCollisionDispatcher*								m_Dispatcher;
	/// btDbvtBroadphase is a good general purpose broadphase . You can also try out btAxis3Sweep .
	btBroadphaseInterface*								m_OverlappingPairCache;
	/// the default constraint solver . For parallel processing you can use a different solver (see Extras / BulletMultiThreaded)
	btSequentialImpulseConstraintSolver*				m_Solver;
	btDiscreteDynamicsWorld*							m_PhysicsWorld;

	//= Debug ===

	BulletDebugDraw										m_DebugDrawer;
	static LineBatcher									sm_LineBatcher;
	bool												m_LogGameObjectRegistering;
	bool												m_LogGameObjectDestruction;

	//= Other =======

	//= Editor =========

#if WITH_EDITOR
	class EditorCamera*									m_EditorCam;
#endif

	//= Controllers =======

	std::vector<Controller*>							m_RegisteredControllers;
};