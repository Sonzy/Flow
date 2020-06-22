#pragma once
#include "Flow\Core.h"
#include <vector>
#include <memory>
#include "Flow\Helper\Maths.h"

#include "btBulletDynamicsCommon.h"

#include "Flow\Helper\BulletDebugDrawing.h"
#include "Flow/GameFramework/Level.h"
#include "Flow/Rendering/Core/DebugDrawing/LineBatcher.h"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

class Actor;
class Skybox;
class Controller;

enum class WorldState
{
	Paused,
	Editor,
	InGame
};

class FLOW_API World
{
public:
	World();
	World(const std::string& WorldName);
	~World();

	void SaveLevel();
	void LoadLevel();

	void SavePlayState();
	void LoadPlayState();

	//TODO: Temp rendering in the level
	void Render();

	void InitialiseWorld();
#if WITH_EDITOR
	void StartEditor();
#endif

	template<typename T>
	std::shared_ptr<T> SpawnActor(const std::string& Name)
	{
		std::shared_ptr<T> NewObject = std::make_shared<T>(Name);
		_MainLevel->GetActors().push_back(NewObject);

		return NewObject;
	}

	void DestroyActor(std::shared_ptr<Actor>& Act);

	void Tick(float DeltaTime);

	const std::string& GetName();

	static btCollisionWorld::ClosestRayResultCallback WorldTrace(Vector Start, Vector End);

	static btDiscreteDynamicsWorld* GetPhysicsWorld();
	static World* GetWorld();

	void AddPhysicsObject(btRigidBody* Obj);
	void AddCollisionObject(btCollisionObject* Obj);

	//= Controllers ========

	void RegisterController(std::shared_ptr<Controller> NewController);
	void DeRegisterController(std::shared_ptr<Controller> OldController);
	Controller* GetLocalController() const;


	//= Getters etc
	LineBatcher& GetLineBatcher() const { return s_LineBatcher; };
	static LineBatcher& GetLineBatcher_S();

	BulletDebugDraw& GetPhysicsDebugDrawer() { return _DebugDrawer; }

	std::vector<std::shared_ptr<Actor>>& GetActors() { return _MainLevel->GetActors(); }

	WorldState GetWorldState() const { return _WorldState; }
protected:
	friend class Application;
	void StartGame();
	void PauseGame();
	void StopGame();


protected:
	void InitialisePhysics();

private:
	friend class Inspector;

	std::string _WorldName;

	Level* _MainLevel;

	WorldState _WorldState;

	//=== World Physics ===

	/* Default memory setup */
	btDefaultCollisionConfiguration* _CollisionConfig;
	/* Default single threaded collision dispatcher */
	btCollisionDispatcher* _Dispatcher;
	/// btDbvtBroadphase is a good general purpose broadphase . You can also try out btAxis3Sweep .
	btBroadphaseInterface* _OverlappingPairCache;
	/// the default constraint solver . For parallel processing you can use a different solver (see Extras / BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* _Solver;
	btDiscreteDynamicsWorld* _PhysicsWorld;

	//= Debug ===

	BulletDebugDraw _DebugDrawer;
	static LineBatcher s_LineBatcher;

	//= Other =======

	//= Editor =========

#if WITH_EDITOR
	 std::shared_ptr<class EditorCamera> _EditorCam;
#endif

	//= Controllers =======

	std::vector<std::shared_ptr<Controller>> _RegisteredControllers;
};