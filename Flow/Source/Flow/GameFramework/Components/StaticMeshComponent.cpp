#include "Flowpch.h"
#include "StaticMeshComponent.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "Flow\GameFramework\World.h"

namespace Flow
{
	StaticMeshComponent::StaticMeshComponent()
		: RenderableComponent("Unnamed StaticMesh Component"), m_StaticMesh(nullptr), m_Material(nullptr),
		Body(nullptr), Collision(nullptr)
	{
	}

	StaticMeshComponent::StaticMeshComponent(const std::string& Name, MeshAsset* Mesh, Material* Material, int MeshIndex)
		: RenderableComponent(Name), m_StaticMesh(nullptr), m_Material(nullptr),
		Body(nullptr), Collision(nullptr)
	{
		if (Mesh && Material)
			SetMeshAndMaterial(Mesh, Material, MeshIndex);
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

		if (Body)// && SimulatePhysics)
		{
			btVector3 Vec = Body->getWorldTransform().getOrigin();
			Rotator Rotation;
			//Body->getWorldTransform().getRotation().getEulerZYX(Rotation.Yaw, Rotation.Pitch, Rotation.Roll);
			//Body->getWorldTransform().getRotation().getEulerZYX(Rotation.Roll, Rotation.Yaw, Rotation.Pitch);
			Body->getWorldTransform().getRotation().getEulerZYX(Rotation.Roll, Rotation.Yaw, Rotation.Pitch);

			//FLOW_ENGINE_LOG("Mesh Position: {0} {1} {2}", Vec.x(), Vec.y(), Vec.z());

			//btScalar m[16];
			//trans.getOpenGLMatrix(m);
			//fAngZ = atan2f(m[1], m[5]);
			//fAngY = atan2f(m[8], m[10]);
			//fAngX = -asinf(m[9]);



			Rotation = Rotator::AsDegrees(Rotation);

			SetWorldLocation(Vector(Vec.x(), Vec.y(), Vec.z()));
			SetWorldRotation(Rotation);

			if(Body->getMass() > 0)
				FLOW_ENGINE_LOG("MovePhysBody: ObjectRot: {0}, BulletRot {1}", GetWorldRotation(), Rotation);
		}
	}

	void StaticMeshComponent::SetMeshAndMaterial(MeshAsset* Mesh, Material* Material, int MeshIndex)
	{
		CHECK_RETURN(!Mesh, "StaticMeshComponent::SetMeshAndMaterial: Mesh was nullptr");
		CHECK_RETURN(!Material, "StaticMeshComponent::SetMeshAndMaterial: Material was nullptr");

		m_StaticMesh = Mesh->GetMesh(MeshIndex);
		m_Material = Material;

		RefreshBinds();
	}

	void StaticMeshComponent::SetStaticMesh(const std::string& MeshName)
	{
		//TODO: Change mesh at runtime without creating new object
		MeshAsset* NewAsset = AssetSystem::GetAsset<MeshAsset>(MeshName);
		CHECK_RETURN(!NewAsset, "StaticMeshComponent::SetStaticMesh: Failed to get new static mesh.");

		m_StaticMesh = NewAsset->GetMesh(0);

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
		auto Vertices = m_StaticMesh->GetCollisionVertices();
		for (auto Vert : Vertices)
		{
			btVector3 btv = btVector3(Vert.X, Vert.Y, Vert.Z);
			Shape->addPoint(btv);
		}

		Collision = Shape;

		Transform trans = GetWorldTransform();
		Collision->setLocalScaling(btVector3(trans.m_Scale.X, trans.m_Scale.Y, trans.m_Scale.Z));
	}

	void StaticMeshComponent::CreateRigidBody()
	{
		//Convert transform into BT Quaternion
		btQuaternion Rotation;
		Rotator Rot = Rotator::AsRadians(GetWorldRotation());
		Vector Pos = GetWorldLocation();
		Rotation.setEulerZYX(Rot.Roll, Rot.Yaw, Rot.Pitch);
		//Rotation.setEulerZYX(Rot.Yaw, Rot.Pitch, Rot.Roll);

		//Setup the motion state
		btVector3 Position = btVector3(Pos.X, Pos.Y, Pos.Z);
		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(Rotation, Position));

		//Initialise the mass and intertia values
		btScalar bodyMass = SimulatePhysics ?  20.0f : 0.0f;
		btVector3 bodyInertia;
		Collision->calculateLocalInertia(bodyMass, bodyInertia);

		//Create the construction info for the body
		btRigidBody::btRigidBodyConstructionInfo bodyCI =
			btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, Collision, bodyInertia);
		bodyCI.m_restitution = 0.4f;
		bodyCI.m_friction = 0.5f;
		bodyCI.m_rollingFriction = 0.2f;
		bodyCI.m_spinningFriction = 0.3f;

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

	void StaticMeshComponent::MovePhysicsBody(Transform NewTransform)
	{
		btMotionState* motionState = Body->getMotionState();
		btTransform Transform;
		btQuaternion Rotation;
		Rotator RadiansRotation = Rotator::AsRadians(NewTransform.m_Rotation);
		//Rotation.setEulerZYX(RadiansRotation.Yaw, RadiansRotation.Pitch, RadiansRotation.Roll);
		Rotation.setEulerZYX(RadiansRotation.Roll, RadiansRotation.Yaw, RadiansRotation.Pitch);

		//Set new transform
		Transform.setOrigin(btVector3(NewTransform.m_Location.X, NewTransform.m_Location.Y, NewTransform.m_Location.Z));
		Transform.setRotation(Rotation);

		//Set Scale
		Collision->setLocalScaling(btVector3(NewTransform.m_Scale.X, NewTransform.m_Scale.Y, NewTransform.m_Scale.Z));

		//Update Transform
		Body->setWorldTransform(Transform);
		motionState->setWorldTransform(Transform);
		World::GetPhysicsWorld()->updateSingleAabb(Body);

		//Re-enable physics body
		Body->activate();
	}
}