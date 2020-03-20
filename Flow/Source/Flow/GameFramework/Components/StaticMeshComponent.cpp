#include "Flowpch.h"
#include "StaticMeshComponent.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

namespace Flow
{
	StaticMeshComponent::StaticMeshComponent()
		: RenderableComponent("Unnamed StaticMesh Component"), m_StaticMesh(nullptr), m_Material(nullptr),
		Body(nullptr), Collision(nullptr)
	{
	}

	StaticMeshComponent::StaticMeshComponent(const std::string& Name, MeshAsset* Mesh, Material* Material)
		: RenderableComponent(Name), m_StaticMesh(nullptr), m_Material(nullptr),
		Body(nullptr), Collision(nullptr)
	{
		if (Mesh && Material)
			SetMeshAndMaterial(Mesh, Material);
	}

	StaticMeshComponent::~StaticMeshComponent()
	{
		delete m_StaticMesh;
	}

	void StaticMeshComponent::InitialiseComponent(MeshAsset* Mesh, Material* Material)
	{
		SetMeshAndMaterial(Mesh, Material);
	}

	void StaticMeshComponent::Tick(float DeltaTime)
	{
		GameObject::Tick(DeltaTime);

		if (Body)
		{
			btVector3 Vec = Body->getWorldTransform().getOrigin();
			Rotator Rotation;
			Body->getWorldTransform().getRotation().getEulerZYX(Rotation.Yaw, Rotation.Pitch, Rotation.Roll);
			//FLOW_ENGINE_LOG("Mesh Position: {0} {1} {2}", Vec.x(), Vec.y(), Vec.z());

			SetWorldLocation(Vector(Vec.x(), Vec.y(), Vec.z()));
			SetWorldRotation(Rotation);
		}
	}

	void StaticMeshComponent::SetMeshAndMaterial(MeshAsset* Mesh, Material* Material)
	{
		CHECK_RETURN(!Mesh, "StaticMeshComponent::SetMeshAndMaterial: Mesh was nullptr");
		CHECK_RETURN(!Material, "StaticMeshComponent::SetMeshAndMaterial: Material was nullptr");

		m_StaticMesh = Mesh;
		m_Material = Material;

		RefreshBinds();
	}

	void StaticMeshComponent::SetStaticMesh(const std::string& MeshName)
	{
		//TODO: Change mesh at runtime without creating new object
		MeshAsset* NewAsset = AssetSystem::GetAsset<MeshAsset>(MeshName);
		CHECK_RETURN(!NewAsset, "StaticMeshComponent::SetStaticMesh: Failed to get new static mesh.");

		m_StaticMesh = NewAsset;

		RefreshBinds(); //TODO: Dont call this twice? dunno
	}

	void StaticMeshComponent::SetMaterial(Material* NewMaterial)
	{
		m_Material = NewMaterial;

		RefreshBinds();
	}

	void StaticMeshComponent::Render()
	{
		Flow::Renderer::Submit(this);

		WorldComponent::Render();
	}

	void StaticMeshComponent::RefreshBinds()
	{
		VertexLayout MeshLayout;
		m_Binds = m_StaticMesh->GenerateBinds(MeshLayout);
		m_IndexBuffer = m_StaticMesh->GetIndexBuffer();

		AddBind(std::make_shared<TransformConstantBuffer>(this));
		m_Material->BindMaterial(this, MeshLayout);
	}

	void StaticMeshComponent::GenerateCollision()
	{
		btConvexHullShape* Shape = new btConvexHullShape();
		auto Vertices = m_StaticMesh->GetVertices();
		for (auto Vert : Vertices)
		{
			btVector3 btv = btVector3(Vert.Position.X, Vert.Position.Y, Vert.Position.Z);
			Shape->addPoint(btv);
		}

		Collision = Shape;
	}

	void StaticMeshComponent::CreateRigidBody()
	{
		//Convert transform into BT Quaternion
		btQuaternion Rotation;
		Rotator Rot = GetWorldRotation();
		Vector Pos = GetWorldLocation();
		Rotation.setEulerZYX(Rot.Yaw, Rot.Pitch, Rot.Roll);

		//Setup the motion state
		btVector3 Position = btVector3(Pos.X, Pos.Y, Pos.Z);
		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(Rotation, Position));

		//Initialise the mass and intertia values
		btScalar bodyMass = SimulatePhysics ?  1.0f : 0.0f;
		btVector3 bodyInertia;
		Collision->calculateLocalInertia(bodyMass, bodyInertia);

		//Create the construction info for the body
		btRigidBody::btRigidBodyConstructionInfo bodyCI =
			btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, Collision, bodyInertia);
		bodyCI.m_restitution = 0.1f;
		bodyCI.m_friction = 0.5f;


		Body = new btRigidBody(bodyCI);
		Body->setUserPointer(this);
		//Body->setLinearFactor(btVector3(1.0f, 1.0f, 0.0f)); //Constraints
	}

	void StaticMeshComponent::InitialisePhysics()
	{
		GenerateCollision();
		CreateRigidBody();
	}
	btRigidBody* StaticMeshComponent::GetRigidBody()
	{
		return Body;
	}
	void StaticMeshComponent::SetSimulatePhysics(bool Simulate)
	{
		SimulatePhysics = Simulate;
	}
}