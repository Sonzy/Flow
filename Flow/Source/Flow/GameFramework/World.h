#pragma once

// Includes ///////////////////////////////////////////////////////

#include <vector>
#include <memory>
#include "Core.h"
#include "Typedefs.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "GameFramework/Level.h"
#include "Framework/Types/Map.h"
#include "Framework/Utils/BulletDebugDrawing.h"
#include "Framework/Utils/ComponentHelper.h"
#include "Rendering/Core/DebugDrawing/LineBatcher.h"
#include "Maths/Vector3.h"

// Forward Declarations ///////////////////////////////////////////

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

// Global Enums ///////////////////////////////////////////////////

enum class WorldState
{
	Paused,
	Editor,
	InGame
};

// Class Definition ///////////////////////////////////////////////

class FLOW_API World
{
public:

	// Public Static Functions ////////////////////////////////////

	static World&										Get();
	static btDiscreteDynamicsWorld*						GetPhysicsWorld();
	static btCollisionWorld::ClosestRayResultCallback	WorldTrace(Vector3 Start, Vector3 End);

public:

	// Public Functions ///////////////////////////////////////////

	World();
	World(const std::string& WorldName);
	~World();

	// Initialisation //

	void						InitialiseWorld();

	// Info //

	const std::string&			GetName();

	// Updates //

	void						Render();
	void						Tick(float DeltaTime);

	// Levels //

	void						SaveLevel();
	void						LoadLevel();
	void						SavePlayState();
	void						LoadPlayState();

	// World State //

	bool						IsGamePaused() const;




#if WITH_EDITOR
	void						StartEditor();
#endif

	//= Actor manipulation = 

	bool						DestroyActor(FGuid guid);
	bool						DestroyComponent(FGuid guid);

	// Object Management //

	Actor*						FindActor(FGuid guid) const;
	Component*					FindComponent(FGuid guid) const;

	template<typename T>
	T* FindActor(FGuid guid) const;

	template<typename T>
	T* FindComponent(FGuid guid) const;

	template<typename T>
	T* SpawnActor(const std::string& Name);

	template<typename T>
	T* DestroyActor(T* act);

	template<typename T>
	T* DestroyComponent(T* comp);

	template<typename T>
	std::vector<T*>	GetAllActorsOfType();





	void												AddPhysicsObject(btRigidBody* Obj);
	void												AddCollisionObject(btCollisionObject* Obj);

	//= Controllers ========

	void												RegisterController(Controller* NewController);
	void												DeRegisterController(Controller* OldController);
	Controller*											GetLocalController() const;

	//= Registration =

	void												RegisterGameObject(GameObject* newObject);
	void												RegisterGameObject(GameObject* newObject, FGuid guid); //Register with specific guid


	//= Getters etc
	LineBatcher&										GetLineBatcher() const	{ return sm_LineBatcher; };
	static LineBatcher&									GetLineBatcher_S();
	BulletDebugDraw&									GetPhysicsDebugDrawer() { return m_DebugDrawer; }

	const Map<FGuid, Actor*>&			GetActorMap()			{ return m_actorMap; }
	const Map<FGuid, Component*>&		GetComponentMap()		{ return m_componentMap; }

	WorldState											GetWorldState() const;

	void												PrintAllPhysicsObjects() const;

	void												StartGame();
	void												PauseGame();
	void												UnpauseGame();
	bool												StopGame();

protected:
	friend class Application;

	//= Protected Functions ===============


	void												InitialisePhysics(bool Force = false);

private:
	friend class Inspector;

	//= Private Variables =================

	std::string											m_WorldName;
	Level*												m_MainLevel;
	WorldState											m_worldState;
	WorldState											m_previousState;

	//= Lookup =

	std::unordered_map<FGuid, Actor*>					m_actorMap;
	std::unordered_map<FGuid, Component*>				m_componentMap;

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
	string												m_defaultSaveFileName;
#endif

	//= Controllers =======

	std::vector<Controller*>							m_RegisteredControllers;
};

// Inline Function Definitions ///////////////////////////////////////////////////////////////

template<typename T>
inline T* World::SpawnActor(const std::string& Name)
{
	static_assert(std::is_base_of<Actor, T>::value, "Trying to spawn a class that isn't an actor");

	T* newActor = new T(Name);
	RegisterGameObject(newActor);

	return newActor;
}

template<typename T>
inline T* World::DestroyActor(T* act)
{
	static_assert(std::is_base_of<Actor, T>::value, "Tried to destroy a component templated with a non-component type");

	return DestroyActor(act->GetGuid()) ? nullptr : act;
}

template<typename T>
inline T* World::DestroyComponent(T* comp)
{
	static_assert(std::is_base_of<Component, T>::value, "Tried to destroy a component templated with a non-component type");

	return DestroyComponent(comp->GetGuid()) ? nullptr : comp;
}

template<typename T>
inline std::vector<T*> World::GetAllActorsOfType()
{
	std::vector<T*> outVec;
	for (auto actor : m_actorMap)
	{
		if (T* found = dynamic_cast<T*>(actor.second))
		{
			outVec.push_back(found);
		}
	}

	return std::move(outVec);
}

template<typename T>
inline T* World::FindActor(FGuid guid) const
{
	return dynamic_cast<T*>(FindActor(guid));
}

template<typename T>
inline T* World::FindComponent(FGuid guid) const
{
	return dynamic_cast<T*>(FindComponent(guid));
}

inline bool World::IsGamePaused() const
{
	return m_worldState == WorldState::Paused;
}

inline WorldState World::GetWorldState() const 
{ 
	return m_worldState;
}