#pragma once
#include "Flow\Helper\Maths.h"

class btCollisionShape;
class btGhostObject;

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

namespace Flow
{
	class Mesh;
	class Material;
	class StaticMeshComponent;
	class World;

	class SelectionGizmo
	{
	public:
		SelectionGizmo();

		//void RefreshBinds();
		void GenerateCollision();

		void UpdatePosition(Vector Position);
		void UpdateRotation(Rotator Rotation);


		void SetScale(Vector Scale);

		void Render();

		Vector GetPosition();

		StaticMeshComponent* ArrowX;
		StaticMeshComponent* ArrowZ;
		StaticMeshComponent* ArrowY;

		Material* ArrowMaterial;

		void SetVisibility(bool Visible);

		bool Visible = false;

		//= Collision ================

		btCollisionShape* XCollision;
		btGhostObject* XGhost;

		btCollisionShape* YCollision;
		btGhostObject* YGhost;

		btCollisionShape* ZCollision;
		btGhostObject* ZGhost;

		void AddCollidersToWorld(World* World);

	private:

		void GenerateCollisionData(StaticMeshComponent* Component, btCollisionShape*& Collider, btGhostObject*& Ghost);
	};
}