#include "Flowpch.h"
#include "SelectionGizmo.h"

#include "Flow\Rendering\Renderer.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"

#include "Flow/GameFramework/World.h"
#include "Flow\Assets\AssetSystem.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

namespace Flow
{
	SelectionGizmo::SelectionGizmo()
	{
		MeshAsset* Meshes = AssetSystem::GetAsset<MeshAsset>("SelectionGizmo");
		Material* Mat = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_FlatColour_White")->GetMaterial();

		//Create and assign mesh components.
		ArrowX_ = new StaticMeshComponent("ArrowX_", Meshes, Mat, 0);
		ArrowY_ = new StaticMeshComponent("ArrowY_", Meshes, Mat, 1);
		ArrowZ_ = new StaticMeshComponent("ArrowZ_", Meshes, Mat, 2);

		//Init the transform
		const int Scale = 1.0f;
		ArrowX_->SetWorldScale(Scale);
		ArrowY_->SetWorldScale(Scale);
		ArrowZ_->SetWorldScale(Scale);

		//Initialise the meshes
		ArrowX_->RefreshBinds();
		ArrowY_->RefreshBinds();
		ArrowZ_->RefreshBinds();
	}

	SelectionGizmo::~SelectionGizmo()
	{
		delete XGhost_;
		delete YGhost_;
		delete ZGhost_;

		delete ArrowX_;
		delete ArrowY_;
		delete ArrowZ_;

		delete XCollision_;
		delete YCollision_;
		delete ZCollision_;
	}


	void SelectionGizmo::GenerateCollision()
	{
		XGhost_ = new btGhostObject();
		YGhost_ = new btGhostObject();
		ZGhost_ = new btGhostObject();

		//Create collision data for each arrow seperately
		GenerateCollisionData(ArrowX_, XCollision_, XGhost_);
		GenerateCollisionData(ArrowY_, YCollision_, YGhost_);
		GenerateCollisionData(ArrowZ_, ZCollision_, ZGhost_);
	}

	void SelectionGizmo::UpdatePosition(Vector Position)
	{
		ArrowX_->SetWorldPosition(Position);
		ArrowY_->SetWorldPosition(Position);
		ArrowZ_->SetWorldPosition(Position);

		btTransform Transform;
		Transform.setOrigin(btVector3(Position.X, Position.Y, Position.Z));
		XGhost_->setWorldTransform(Transform);
		YGhost_->setWorldTransform(Transform);
		ZGhost_->setWorldTransform(Transform);

		World::GetPhysicsWorld()->updateSingleAabb(XGhost_);
		World::GetPhysicsWorld()->updateSingleAabb(YGhost_);
		World::GetPhysicsWorld()->updateSingleAabb(ZGhost_);
	}

	void SelectionGizmo::UpdateRotation(Rotator Rotation)
	{
	}

	void SelectionGizmo::SetScale(Vector Scale)
	{
		ArrowX_->SetRelativeScale(Scale);
		ArrowY_->SetRelativeScale(Scale);
		ArrowZ_->SetRelativeScale(Scale);
	}

	void SelectionGizmo::Render()
	{
		if (!Visible_)
			return;

		Flow::Renderer::Submit(ArrowX_);
		Flow::Renderer::Submit(ArrowY_);
		Flow::Renderer::Submit(ArrowZ_);
	}

	Vector SelectionGizmo::GetPosition() const
	{
		btVector3 T = XGhost_->getWorldTransform().getOrigin();
		return Vector(T.x(), T.y(), T.z());
	}

	void SelectionGizmo::SetVisibility(bool Visible)
	{
		this->Visible_ = Visible;
	}

	void SelectionGizmo::AddCollidersToWorld(World* World)
	{
		//World->AddCollisionObject(XGhost_);
		//World->AddCollisionObject(YGhost_);
		//World->AddCollisionObject(ZGhost_);
	}

	void SelectionGizmo::GenerateCollisionData(StaticMeshComponent* Component, btCollisionShape*& Collider, btGhostObject*& Ghost)
	{
		//Generate the collision
		btConvexHullShape* Shape = new btConvexHullShape();
		auto Vertices = Component->GetMesh()->GetVertices();
		for (auto Vert : Vertices)
		{
			btVector3 btv = btVector3(Vert.Position_.X, Vert.Position_.Y, Vert.Position_.Z);
			Shape->addPoint(btv);
		}
		Collider = Shape;

		//Create the ghost object
		btTransform Transform;
		Vector Location = Component->GetWorldPosition();
		Transform.setOrigin(btVector3(Location.X, Location.Y, Location.Z));
		Ghost->setCollisionShape(Collider);
		Ghost->setWorldTransform(Transform);
		Ghost->setUserPointer(this);
	}
}