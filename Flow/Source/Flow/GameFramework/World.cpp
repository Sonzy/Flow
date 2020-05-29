#include "Flowpch.h"
#include "World.h"
#include "WorldObject.h"

#include "Flow\Application.h"

#include "ThirdParty\Bullet\LinearMath\btIDebugDraw.h"

#include "Flow\GameFramework\Other\Skybox.h"

#include "Flow\GameFramework\Controllers\Controller.h"
#include "Flow\GameFramework\Controllers\PlayerController.h"

LineBatcher World::s_LineBatcher = LineBatcher();

World::World()
	: World("Unnamed World")
{	}

World::World(const std::string& WorldName)
	: _WorldName(WorldName)
{
	_DebugDrawer.Init();
	s_LineBatcher.Initialise();
}

World::~World()
{
	_WorldObjects.clear();
}

void World::InitialiseWorld()
{
	InitialisePhysics();

	_Skybox = new Skybox();

	std::shared_ptr<PlayerController> NewLocalController = SpawnWorldObject<PlayerController>("NewLocalController");
	_RegisteredControllers.push_back(NewLocalController);
}

void World::DispatchBeginPlay()
{
	for (auto& WorldObj : _WorldObjects)
	{
		WorldObj->BeginPlay();
	}
}

LineBatcher& World::GetLineBatcher_S()
{
	return Application::GetWorld()->s_LineBatcher;
}

void World::InitialisePhysics()
{
	/* Create the physics world with default settings */
	_CollisionConfig = new btDefaultCollisionConfiguration();
	_Dispatcher = new btCollisionDispatcher(_CollisionConfig);
	_OverlappingPairCache = new btDbvtBroadphase();
	_Solver = new btSequentialImpulseConstraintSolver;
	_PhysicsWorld = new btDiscreteDynamicsWorld(_Dispatcher, _OverlappingPairCache, _Solver, _CollisionConfig);

	/* Initialise Physics world properties */
	_PhysicsWorld->setGravity(btVector3(0, -9.81, 0));

	_PhysicsWorld->setDebugDrawer(&_DebugDrawer);
	_PhysicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
}

void World::Tick(float DeltaTime)
{
	{
		PROFILE_CURRENT_SCOPE("Update Physics World");

		_PhysicsWorld->stepSimulation(DeltaTime, 0);
	}

	for (auto& WorldObj : _WorldObjects)
	{
		WorldObj->Tick(DeltaTime);
	}

	Renderer::Submit(_Skybox);
}
const std::string& World::GetName()
{
	return _WorldName;
}

btCollisionWorld::ClosestRayResultCallback World::WorldTrace(Vector Start, Vector End)
{
	btVector3 InternalStart = btVector3(Start.X, Start.Y, Start.Z);
	btVector3 InternaEnd = btVector3(End.X, End.Y, End.Z);

	btCollisionWorld::ClosestRayResultCallback Result(InternalStart, InternaEnd);
	Application::GetWorld()->_PhysicsWorld->rayTest(InternalStart, InternaEnd, Result);

	return Result;
}

btDiscreteDynamicsWorld* World::GetPhysicsWorld()
{
	return Application::GetWorld()->_PhysicsWorld;
}

World* World::GetWorld()
{
	return Application::GetWorld();
}

void World::AddPhysicsObject(btRigidBody* Obj)
{
	_PhysicsWorld->addRigidBody(Obj);
}

void World::AddCollisionObject(btCollisionObject* Obj)
{
	_PhysicsWorld->addCollisionObject(Obj);
}

void World::RegisterController(std::shared_ptr<Controller> NewController)
{
	CHECK_RETURN(!NewController, "Layer::DeRegisterController: Tried to register null controller");
	_RegisteredControllers.push_back(NewController);
}

void World::DeRegisterController(std::shared_ptr<Controller> OldController)
{
	CHECK_RETURN(!OldController, "Layer::DeRegisterController: Tried to deregister null controller");

	auto FoundIterator = std::find(_RegisteredControllers.begin(), _RegisteredControllers.end(), OldController);

	if (FoundIterator == _RegisteredControllers.end())
		FLOW_ENGINE_WARNING("Tried to remove controller ");

	_RegisteredControllers.erase(FoundIterator);
}

Controller* World::GetLocalController() const
{
	for (auto& Controller : _RegisteredControllers)
	{
		if (Controller->IsLocalController())
			return Controller.get();
	}

	return nullptr;
}