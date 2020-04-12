#include "Flowpch.h"
#include "World.h"
#include "WorldObject.h"

#include "Flow\Application.h"

#include "ThirdParty\Bullet\LinearMath\btIDebugDraw.h"

#include "Flow\GameFramework\Other\Skybox.h"

namespace Flow
{
	World::World()
		: World("Unnamed World")
	{	}

	World::World(const std::string& WorldName)
		: WorldName_(WorldName)
	{
		DebugDrawer_.Init();
	}

	World::~World()
	{
		WorldObjects_.clear();
	}

	void World::InitialiseWorld()
	{
		InitialisePhysics();

		Skybox_ = new Skybox();
	}

	void World::DispatchBeginPlay()
	{
		for (auto& WorldObj : WorldObjects_)
		{
			WorldObj->BeginPlay();
		}
	}

	void World::InitialisePhysics()
	{
		/* Create the physics world */
		CollisionConfig_ = new btDefaultCollisionConfiguration();
		Dispatcher_ = new btCollisionDispatcher(CollisionConfig_);
		OverlappingPairCache_ = new btDbvtBroadphase();
		Solver_ = new btSequentialImpulseConstraintSolver;
		PhysicsWorld_ = new btDiscreteDynamicsWorld(Dispatcher_, OverlappingPairCache_, Solver_, CollisionConfig_);

		/* Initialise Physics world properties */
		PhysicsWorld_->setGravity(btVector3(0, -9.81, 0));

		PhysicsWorld_->setDebugDrawer(&DebugDrawer_);
		PhysicsWorld_->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	}

	void World::Tick(float DeltaTime)
	{
		PhysicsWorld_->stepSimulation(DeltaTime, 0);
		for (auto& WorldObj : WorldObjects_)
		{
			WorldObj->Tick(DeltaTime);
		}

		Flow::Renderer::Submit(Skybox_);
	}
	const std::string& World::GetName()
	{
		return WorldName_;
	}

	btCollisionWorld::ClosestRayResultCallback World::WorldTrace(Vector Start, Vector End)
	{
		btVector3 InternalStart = btVector3(Start.X, Start.Y, Start.Z);
		btVector3 InternaEnd = btVector3(End.X, End.Y, End.Z);

		btCollisionWorld::ClosestRayResultCallback Result(InternalStart, InternaEnd);
		Application::GetWorld()->PhysicsWorld_->rayTest(InternalStart, InternaEnd, Result);

		return Result;
	}

	btDiscreteDynamicsWorld* World::GetPhysicsWorld()
	{
		return Application::GetWorld()->PhysicsWorld_;
	}

	World* World::GetWorld()
	{
		return Application::GetWorld();
	}

	void World::AddPhysicsObject(btRigidBody* Obj)
	{
		PhysicsWorld_->addRigidBody(Obj);
	}

	void World::AddCollisionObject(btCollisionObject* Obj)
	{
		PhysicsWorld_->addCollisionObject(Obj);
	}
}