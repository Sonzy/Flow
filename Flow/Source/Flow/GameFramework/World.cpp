#include "Flowpch.h"
#include "World.h"
#include "WorldObject.h"

#include "btBulletDynamicsCommon.h"

namespace Flow
{
	World::World()
		: World("Unnamed World")
	{	}

	World::World(const std::string& WorldName)
		: m_WorldName(WorldName)
	{
		InitialisePhysics();
	}

	World::~World()
	{
		m_WorldObjects.clear();
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
		PhysicsWorld->setGravity(btVector3(0, -10, 0));

		btCollisionObject* CollObj = PhysicsWorld->getCollisionObjectArray()[0];
		btRigidBody* Body = btRigidBody::upcast(CollObj);
	}

	void World::Tick(float DeltaTime)
	{
		for (auto& WorldObj : m_WorldObjects)
		{
			WorldObj->Tick(DeltaTime);
		}
	}
	const std::string& World::GetName()
	{
		return m_WorldName;
	}
}