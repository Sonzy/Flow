#pragma once
#include "Flow\Helper\Maths.h"

#include "Flow\Rendering\Core\Bindable.h"
#include "Flow\Rendering\Core\Bindables\IndexBuffer.h"

class btCollisionShape;
class btGhostObject;

namespace Flow
{
	class Mesh;
	class Material;
	class StaticMeshComponent;
	class World;

	/* Gizmo used for moving objects within the world within the editor */
	class SelectionGizmo
	{
	public:
		SelectionGizmo();
		~SelectionGizmo();

		/* Generates the collision for all parts of the gizmo*/
		void GenerateCollision();

		void UpdatePosition(Vector Position);
		void UpdateRotation(Rotator Rotation);
		void SetScale(Vector Scale);

		void Render();

		[[nodiscard]] Vector GetPosition()  const;

		void SetVisibility(bool Visible);

		/* Adds the collision data to the physics world. */
		void AddCollidersToWorld(World* World);

	private:
		void GenerateCollisionData(StaticMeshComponent* Component, btCollisionShape*& Collider, btGhostObject*& Ghost);

	private:

		StaticMeshComponent* ArrowX_;
		StaticMeshComponent* ArrowY_;
		StaticMeshComponent* ArrowZ_;

		Material* ArrowMaterial_;

		bool Visible_ = false;

		//= Collision ================

		btCollisionShape* XCollision_;
		btCollisionShape* YCollision_;
		btCollisionShape* ZCollision_;
		btGhostObject* XGhost_;
		btGhostObject* YGhost_;
		btGhostObject* ZGhost_;
	};
}