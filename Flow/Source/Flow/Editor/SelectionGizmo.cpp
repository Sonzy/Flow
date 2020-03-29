#include "Flowpch.h"
#include "SelectionGizmo.h"

#include "Flow\Rendering\Renderer.h"
#include "Flow\Assets\Meshes\MeshAsset.h"

#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"

#include "Flow\Assets\AssetSystem.h"

#include "Flow\GameFramework\Components\StaticMeshComponent.h"

#include "Flow/GameFramework/World.h"



namespace Flow
{
	SelectionGizmo::SelectionGizmo()
	{
		MeshAsset* Meshes = AssetSystem::GetAsset<MeshAsset>("SelectionGizmo");
		//MeshAsset* Meshes = AssetSystem::GetAsset<MeshAsset>("Gizmo");
		//Material* Mat = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_FlatColour")->GetMaterial();
		Material* Mat = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_HatFancyMan")->GetMaterial();
		ArrowX = new StaticMeshComponent("ArrowX", Meshes, Mat, 0);
		ArrowY = new StaticMeshComponent("ArrowY", Meshes, Mat, 1);
		ArrowZ = new StaticMeshComponent("ArrowZ", Meshes, Mat, 2);

		const int Scale = 1.0f;

		ArrowX->SetWorldScale(Scale);
		ArrowY->SetWorldScale(Scale);
		ArrowZ->SetWorldScale(Scale);

		//ArrowX->SetWorldRotation(Rotator(90.0f, 90.0f, 0.0f));
		//ArrowY->SetWorldRotation(Rotator(0.0f, 180.0f, 0.0f));
		//ArrowZ->SetWorldRotation(Rotator(0, 0.0f, 0.0f));

		ArrowX->RefreshBinds();
		ArrowY->RefreshBinds();
		ArrowZ->RefreshBinds();
	}	

	void SelectionGizmo::GenerateCollision()
	{
		XGhost = new btGhostObject();
		YGhost = new btGhostObject();
		ZGhost = new btGhostObject();

		GenerateCollisionData(ArrowX, XCollision, XGhost);
		GenerateCollisionData(ArrowY, YCollision, YGhost);
		GenerateCollisionData(ArrowZ, ZCollision, ZGhost);
	}

	void SelectionGizmo::UpdatePosition(Vector Position)
	{
		ArrowX->SetWorldLocation(Position);
		ArrowY->SetWorldLocation(Position);
		ArrowZ->SetWorldLocation(Position);

		btTransform Transform;
		Transform.setOrigin(btVector3(Position.X, Position.Y, Position.Z));
		XGhost->setWorldTransform(Transform);
		YGhost->setWorldTransform(Transform);
		ZGhost->setWorldTransform(Transform);

		World::GetPhysicsWorld()->updateSingleAabb(XGhost);
		World::GetPhysicsWorld()->updateSingleAabb(YGhost);
		World::GetPhysicsWorld()->updateSingleAabb(ZGhost);
	}

	void SelectionGizmo::UpdateRotation(Rotator Rotation)
	{
	}

	void SelectionGizmo::SetScale(Vector Scale)
	{
		ArrowX->SetRelativeScale(Scale);
		ArrowY->SetRelativeScale(Scale);
		ArrowZ->SetRelativeScale(Scale);
	}

	void SelectionGizmo::Render()
	{
		if (!Visible)
			return;

		Flow::Renderer::Submit(ArrowX);
		Flow::Renderer::Submit(ArrowY);
		Flow::Renderer::Submit(ArrowZ);
	}

	Vector SelectionGizmo::GetPosition()
	{
		btVector3 T = XGhost->getWorldTransform().getOrigin();
		return Vector(T.x(), T.y(), T.z());
		//return ArrowX->GetWorldLocation();
	}

	void SelectionGizmo::SetVisibility(bool Visible)
	{
		this->Visible = Visible;
	}

	void SelectionGizmo::AddCollidersToWorld(World* World)
	{
		//World->AddCollisionObject(XGhost);
		//World->AddCollisionObject(YGhost);
		//World->AddCollisionObject(ZGhost);
	}

	void SelectionGizmo::GenerateCollisionData(StaticMeshComponent* Component, btCollisionShape*& Collider, btGhostObject*& Ghost)
	{
		//Generate the collision
		btConvexHullShape* Shape = new btConvexHullShape();
		auto Vertices = Component->GetMesh()->GetVertices();
		for (auto Vert : Vertices)
		{
			btVector3 btv = btVector3(Vert.Position.X, Vert.Position.Y, Vert.Position.Z);
			Shape->addPoint(btv);
		}
		Collider = Shape;

		//Create the ghost object

		Ghost->setCollisionShape(Collider);
		btTransform XTransform;
		Vector Trans = Component->GetWorldLocation();
		XTransform.setOrigin(btVector3(Trans.X, Trans.Y, Trans.Z));
		Ghost->setWorldTransform(XTransform);
		Ghost->setUserPointer(this);
	}
}