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

//= Forward Declarations =================================

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class Actor;
class Skybox;
class Controller;

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
		T* NewObject = new T(Name);
		m_MainLevel->GetActors().push_back(NewObject);

		return NewObject;
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

	/* Adds an actor to the world that was default created, takes ownership*/
	void												AddDefaultInitialisedActor(Actor* NewActor);

	void												DestroyActor(Actor* Act);

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


	//= Getters etc
	LineBatcher&										GetLineBatcher() const { return sm_LineBatcher; };
	static LineBatcher&									GetLineBatcher_S();
	BulletDebugDraw&									GetPhysicsDebugDrawer() { return m_DebugDrawer; }
	std::vector<Actor*>&								GetActors(){ return m_MainLevel->GetActors(); }

	WorldState											GetWorldState() const { return m_WorldState; }

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

	//= Other =======

	//= Editor =========

#if WITH_EDITOR
	class EditorCamera*									m_EditorCam;
#endif

	//= Controllers =======

	std::vector<Controller*>							m_RegisteredControllers;
};