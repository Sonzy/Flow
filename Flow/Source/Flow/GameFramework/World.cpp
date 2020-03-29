#include "Flowpch.h"
#include "World.h"
#include "WorldObject.h"

#include "Flow\Application.h"

#include "ThirdParty\Bullet\LinearMath\btIDebugDraw.h"

namespace Flow
{
	World::World()
		: World("Unnamed World")
	{	}

	World::World(const std::string& WorldName)
		: m_WorldName(WorldName)
	{
		DebugDrawer.Init();
	}

	World::~World()
	{
		m_WorldObjects.clear();
	}

	void World::InitialiseWorld()
	{
		InitialisePhysics();

		//TODO: Temp, initialise object physics
		//for (auto& WorldObj : m_WorldObjects)
		//{
		//	if (WorldObj->CollisionEnabled())
		//	{
		//		WorldObj->InitialisePhysics();
		//		PhysicsWorld->addRigidBody(WorldObj->GetRigidBody());		
		//	}
		//}
	}

	void World::DispatchBeginPlay()
	{
		for (auto& WorldObj : m_WorldObjects)
		{
			WorldObj->BeginPlay();
		}
	}

	void World::InitialisePhysics()
	{
		/* Create the physics world */
		CollisionConfig = new btDefaultCollisionConfiguration();
		Dispatcher = new btCollisionDispatcher(CollisionConfig);
		OverlappingPairCache = new btDbvtBroadphase();
		Solver = new btSequentialImpulseConstraintSolver;
		PhysicsWorld = new btDiscreteDynamicsWorld(Dispatcher, OverlappingPairCache, Solver, CollisionConfig);

		/* Initialise Physics world properties */
		PhysicsWorld->setGravity(btVector3(0, -9.81, 0));

		PhysicsWorld->setDebugDrawer(&DebugDrawer);
		PhysicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	}

	void World::Tick(float DeltaTime)
	{
		Line::Count = 0;
		PhysicsWorld->stepSimulation(DeltaTime);
		for (auto& WorldObj : m_WorldObjects)
		{
			WorldObj->Tick(DeltaTime);
		}
	}
	const std::string& World::GetName()
	{
		return m_WorldName;
	}

	btCollisionWorld::ClosestRayResultCallback World::WorldTrace(Vector Start, Vector End)
	{
		btVector3 InternalStart = btVector3(Start.X, Start.Y, Start.Z);
		btVector3 InternaEnd = btVector3(End.X, End.Y, End.Z);

		btCollisionWorld::ClosestRayResultCallback Result(InternalStart, InternaEnd);
		Application::GetWorld()->PhysicsWorld->rayTest(InternalStart, InternaEnd, Result);

		return Result;
	}

	btDiscreteDynamicsWorld* World::GetPhysicsWorld()
	{
		return Application::GetWorld()->PhysicsWorld;
	}

	World* World::GetWorld()
	{
		return Application::GetWorld();
	}

	void World::AddPhysicsObject(btRigidBody* Obj)
	{
		PhysicsWorld->addRigidBody(Obj);
	}

	void World::AddCollisionObject(btCollisionObject* Obj)
	{
		PhysicsWorld->addCollisionObject(Obj);
	}
}