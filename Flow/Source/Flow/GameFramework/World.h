#pragma once
#include "Flow\Core.h"
#include <vector>
#include <memory>
#include "Flow\Helper\Maths.h"

#include "btBulletDynamicsCommon.h"

#include "Flow\Helper\BulletDebugDrawing.h"
#include "Flow/Rendering/Core/DebugDrawing/LineBatcher.h"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

class WorldObject;
class Skybox;
class Controller;

class FLOW_API World
{
public:
	World();
	World(const std::string& WorldName);
	~World();

	void InitialiseWorld();

	void DispatchBeginPlay();

	template<typename T>
	std::shared_ptr<T> SpawnWorldObject(const std::string& Name)
	{
		std::shared_ptr<T> NewObject = std::make_shared<T>(Name);
		_WorldObjects.push_back(NewObject);

		return NewObject;
	}

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

protected:
	void InitialisePhysics();

private:
	friend class Inspector;

	std::vector<std::shared_ptr<WorldObject>> _WorldObjects;
	std::string _WorldName;

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
	Skybox* _Skybox;

	//= Controllers =======

	std::vector<std::shared_ptr<Controller>> _RegisteredControllers;
};