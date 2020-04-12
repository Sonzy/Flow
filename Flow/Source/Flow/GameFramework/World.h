#pragma once
#include "Flow\Core.h"
#include <vector>
#include <memory>
#include "Flow\Helper\Maths.h"

#include "btBulletDynamicsCommon.h"

#include "Flow\Helper\BulletDebugDrawing.h"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace Flow
{
	class WorldObject;
	class Skybox;

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
			WorldObjects_.push_back(NewObject);

			return NewObject;
		}

		void Tick(float DeltaTime);

		const std::string& GetName();

		static btCollisionWorld::ClosestRayResultCallback WorldTrace(Vector Start, Vector End);

		static btDiscreteDynamicsWorld* GetPhysicsWorld();
		static World* GetWorld();

		void AddPhysicsObject(btRigidBody* Obj);
		void AddCollisionObject(btCollisionObject* Obj);

	protected:
		void InitialisePhysics();

	private:
		friend class Inspector;

		std::vector<std::shared_ptr<WorldObject>> WorldObjects_;
		std::string WorldName_;

		//=== World Physics ===

		/* Default memory setup */
		btDefaultCollisionConfiguration* CollisionConfig_;
		/* Default single threaded collision dispatcher */
		btCollisionDispatcher* Dispatcher_;
		/// btDbvtBroadphase is a good general purpose broadphase . You can also try out btAxis3Sweep .
		btBroadphaseInterface* OverlappingPairCache_;
		/// the default constraint solver . For parallel processing you can use a different solver (see Extras / BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* Solver_;
		btDiscreteDynamicsWorld* PhysicsWorld_;

		BulletDebugDraw DebugDrawer_;

		//= Other =======
		Skybox* Skybox_;
	};
}