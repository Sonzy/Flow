#pragma once
#include "Flow\Core.h"
#include <vector>
#include <memory>
#include "Flow\Helper\Maths.h"

#include "btBulletDynamicsCommon.h"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace Flow
{
	class WorldObject;

	class FLOW_API World
	{
	public:
		World();
		World(const std::string& WorldName);
		~World();

		void InitialiseWorld();

		template<typename T>
		std::shared_ptr<T> SpawnWorldObject()
		{
			std::shared_ptr<T> NewObject = std::make_shared<T>();
			m_WorldObjects.push_back(NewObject);

			return NewObject;
		}

		void Tick(float DeltaTime);

		const std::string& GetName();

		static btCollisionWorld::ClosestRayResultCallback WorldTrace(Vector Start, Vector End);

		static btDiscreteDynamicsWorld* GetPhysicsWorld();
		static World* GetWorld();

		void AddPhysicsObject(btCollisionObject* Obj);

	protected:
		void InitialisePhysics();

	private:
		friend class Inspector;

		std::vector<std::shared_ptr<WorldObject>> m_WorldObjects;
		std::string m_WorldName;

		//=== World Physics ===

		/* Default memory setup */
		btDefaultCollisionConfiguration* CollisionConfig;
		/* Default single threaded collision dispatcher */
		btCollisionDispatcher* Dispatcher;
		/// btDbvtBroadphase is a good general purpose broadphase . You can also try out btAxis3Sweep .
		btBroadphaseInterface* OverlappingPairCache;
		/// the default constraint solver . For parallel processing you can use a different solver (see Extras / BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* Solver;
		btDiscreteDynamicsWorld* PhysicsWorld;
	};
}