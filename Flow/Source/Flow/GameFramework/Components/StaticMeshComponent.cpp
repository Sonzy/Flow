#include "Flowpch.h"
#include "StaticMeshComponent.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"
#include "Flow\Rendering\Core\Bindables\Stencil.h"
#include "Flow\Rendering\Core\Bindables\Rasterizer.h"

#include "ThirdParty\ImGui\imgui.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "Flow\GameFramework\World.h"

namespace Flow
{
	StaticMeshComponent::StaticMeshComponent()
		: RenderableComponent("Unnamed StaticMesh Component"), StaticMesh_(nullptr), Material_(nullptr),
		Body_(nullptr), Collision_(nullptr)
	{
	}

	StaticMeshComponent::StaticMeshComponent(const std::string& Name, MeshAsset* Mesh, Material* Material, int MeshIndex)
		: RenderableComponent(Name), StaticMesh_(nullptr), Material_(nullptr),
		Body_(nullptr), Collision_(nullptr)
	{
		if (Mesh && Material)
			SetMeshAndMaterial(Mesh, Material, MeshIndex);
	}

	StaticMeshComponent::~StaticMeshComponent()
	{
		delete StaticMesh_;
		delete Collision_;
	}

	void StaticMeshComponent::InitialiseComponent(MeshAsset* Mesh, Material* Material)
	{
		SetMeshAndMaterial(Mesh, Material);
	}

	void StaticMeshComponent::Tick(float DeltaTime)
	{
		WorldComponent::Tick(DeltaTime);
		
		if (Body_ && SimulatePhysics_)
		{
			btVector3 Vec = Body_->getWorldTransform().getOrigin();

			//Need euler YZX 
			btScalar m[16];
			Body_->getWorldTransform().getOpenGLMatrix(m);
			float fAngZ = atan2f(m[1], m[5]);
			float fAngY = atan2f(m[8], m[10]);
			float fAngX = -asinf(m[9]);

			SetWorldPosition(Vector(Vec.x(), Vec.y(), Vec.z()));
			SetWorldRotation(Rotator::AsDegrees(Rotator(fAngX, fAngZ, fAngY)));
		}

		if (!SimulatePhysics_ && Body_)
		{
			MovePhysicsBody(GetWorldTransform());
		}
	}

	void StaticMeshComponent::SetMeshAndMaterial(MeshAsset* Mesh, Material* Material, int MeshIndex)
	{
		CHECK_RETURN(!Mesh, "StaticMeshComponent::SetMeshAndMaterial: Mesh was nullptr");
		CHECK_RETURN(!Material, "StaticMeshComponent::SetMeshAndMaterial: Material was nullptr");

		StaticMesh_ = Mesh->GetMesh(MeshIndex);
		Material_ = Material;

		RefreshBinds();
	}

	void StaticMeshComponent::SetStaticMesh(const std::string& MeshName)
	{
		//TODO: Change mesh at runtime without creating new object
		MeshAsset* NewAsset = AssetSystem::GetAsset<MeshAsset>(MeshName);
		CHECK_RETURN(!NewAsset, "StaticMeshComponent::SetStaticMesh: Failed to get new static mesh.");
		
		StaticMesh_ = NewAsset->GetMesh(0);

		RefreshBinds(); //TODO: Dont call this twice? dunno
	}

	void StaticMeshComponent::SetMaterial(Material* NewMaterial)
	{
		Material_ = NewMaterial;

		RefreshBinds();
	}

	void StaticMeshComponent::Render()
	{
		Flow::Renderer::Submit(this);

		if (DrawOutline_)
			DrawOutline();

		WorldComponent::Render();
	}

	void StaticMeshComponent::RefreshBinds()
	{
		VertexLayout MeshLayout;
		Binds_ = StaticMesh_->GenerateBinds(MeshLayout);
		IndexBuffer_ = StaticMesh_->GetIndexBuffer();

		auto Transform = std::make_shared<TransformConstantBuffer>(this);
		AddBind(Transform);
		Material_->BindMaterial(this, MeshLayout);

		AddBind(Rasterizer::Resolve(false));

		//TODO: Temp stencil testing
		if (DrawOutline_)
		{
			AddBind(std::make_shared<Stencil>(StencilMode::Write));
			VertexBuffer Dummy = VertexBuffer(MeshLayout);
			std::vector<unsigned short> Dummy2;

			OutlineEffect.push_back(BindableVertexBuffer::Resolve(StaticMesh_->Parent_->GetAssetName() + std::to_string(StaticMesh_->MeshIndex_), Dummy));
			OutlineEffect.push_back(IndexBuffer::Resolve(StaticMesh_->Parent_->GetAssetName() + std::to_string(StaticMesh_->MeshIndex_), Dummy2));

			auto VS = VertexShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("SolidColourVS")->GetPath());
			auto VSByteCode = static_cast<VertexShader&>(*VS).GetByteCode();
			OutlineEffect.push_back(std::move(VS));
			OutlineEffect.push_back(PixelShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("SolidColourPS")->GetPath()));
			struct SolidColorBuffer
			{
				DirectX::XMFLOAT4 color = { 1.0f,0.4f,0.4f,1.0f };
			} scb;
			OutlineEffect.push_back(PixelConstantBuffer<SolidColorBuffer>::Resolve(scb, 2u));

			OutlineEffect.push_back(InputLayout::Resolve(MeshLayout, VSByteCode));
			OutlineEffect.push_back(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			OutlineEffect.push_back(Transform);
			OutlineEffect.push_back(std::make_shared<Stencil>(StencilMode::Mask));
		}
		else
			AddBind(std::make_shared<Stencil>(StencilMode::Off));
	}

	DirectX::XMMATRIX StaticMeshComponent::GetTransformXM()
	{
		Transform WorldTransform = GetWorldTransform();
		Rotator RadianRotation = Rotator::AsRadians(WorldTransform.Rotation_);

		Vector Scale = WorldTransform.Scale_;
		if (CurrentlyOutlining)
			Scale += OutlineThickness_;

		auto Trans = DirectX::XMMatrixScaling(Scale.X, Scale.Y, Scale.Z) *
			DirectX::XMMatrixRotationRollPitchYaw(RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll) *
			DirectX::XMMatrixTranslation(WorldTransform.Position_.X, WorldTransform.Position_.Y, WorldTransform.Position_.Z);

		return Trans;
	}

	void StaticMeshComponent::DrawOutline()
	{
		CurrentlyOutlining = true;
		for (auto& b : OutlineEffect)
		{
			b->Bind();
		}
		RenderCommand::DrawIndexed(GetIndexBuffer().GetCount());
		CurrentlyOutlining = false;
	}

	void StaticMeshComponent::DrawComponentDetailsWindow()
	{
		WorldComponent::DrawComponentDetailsWindow();

		bool BindsDirty = false;
		BindsDirty |= ImGui::Checkbox("Outline", &DrawOutline_);
		BindsDirty |= ImGui::InputFloat("Outline Scale", &OutlineThickness_, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue);

		if (BindsDirty)
			RefreshBinds();
	}

	void StaticMeshComponent::GenerateCollision()
	{
		delete Collision_;

		btConvexHullShape* Shape = new btConvexHullShape();
		auto Vertices = StaticMesh_->GetCollisionVertices();
		for (auto Vert : Vertices)
		{
			btVector3 btv = btVector3(Vert.X, Vert.Y, Vert.Z);
			Shape->addPoint(btv);
		}

		Collision_ = Shape;
		Collision_->setMargin(0.01f);

		Transform trans = GetWorldTransform();
		Collision_->setLocalScaling(btVector3(trans.Scale_.X, trans.Scale_.Y, trans.Scale_.Z));
	}

	void StaticMeshComponent::CreateRigidBody()
	{
		//Convert transform into BT Quaternion
		btQuaternion Rotation;
		Rotator Rot = Rotator::AsRadians(GetWorldRotation());
		Vector Pos = GetWorldPosition();
		Rotation.setEulerZYX(Rot.Roll, Rot.Yaw, Rot.Pitch);

		//Setup the motion state
		btVector3 Position = btVector3(Pos.X, Pos.Y, Pos.Z);
		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(Rotation, Position));

		//Initialise the mass and intertia values
		btScalar bodyMass = SimulatePhysics_ ?  20.0f : 0.0f;
		btVector3 bodyInertia;
		Collision_->calculateLocalInertia(bodyMass, bodyInertia);

		//Create the construction info for the body
		btRigidBody::btRigidBodyConstructionInfo bodyCI =
			btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, Collision_, bodyInertia);
		bodyCI.m_restitution = 0.4f;
		bodyCI.m_friction = 0.5f;
		bodyCI.m_rollingFriction = 0.2f;
		bodyCI.m_spinningFriction = 0.3f;

		Body_ = new btRigidBody(bodyCI);
		Body_->setUserPointer(this);
	}

	void StaticMeshComponent::InitialisePhysics()
	{
		GenerateCollision();
		CreateRigidBody();
	}

	btRigidBody* StaticMeshComponent::GetRigidBody()
	{
		return Body_;
	}

	void StaticMeshComponent::SetSimulatePhysics(bool Simulate)
	{
		SimulatePhysics_ = Simulate;
	}

	void StaticMeshComponent::MovePhysicsBody(Transform NewTransform)
	{
		btMotionState* motionState = Body_->getMotionState();
		btTransform Transform;
		btQuaternion Rotation;
		Rotator RadiansRotation = Rotator::AsRadians(NewTransform.Rotation_);
		Rotation.setEuler(RadiansRotation.Yaw, RadiansRotation.Pitch, RadiansRotation.Roll);

		//Set new transform
		Transform.setOrigin(btVector3(NewTransform.Position_.X, NewTransform.Position_.Y, NewTransform.Position_.Z));
		Transform.setRotation(Rotation);

		//Set Scale
		Collision_->setLocalScaling(btVector3(NewTransform.Scale_.X, NewTransform.Scale_.Y, NewTransform.Scale_.Z));

		//Update Transform
		Body_->setWorldTransform(Transform);
		motionState->setWorldTransform(Transform);
		World::GetPhysicsWorld()->updateSingleAabb(Body_);

		//Re-enable physics body
		Body_->activate();
	}
}