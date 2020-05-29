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

StaticMeshComponent::StaticMeshComponent()
	: RenderableComponent("Unnamed StaticMesh Component"), _StaticMesh(nullptr), _Material(nullptr),
	_Body(nullptr), _Collision(nullptr)
{
}

StaticMeshComponent::StaticMeshComponent(const std::string& Name, MeshAsset* Mesh, Material* Material, int MeshIndex)
	: RenderableComponent(Name), _StaticMesh(nullptr), _Material(nullptr),
	_Body(nullptr), _Collision(nullptr)
{
	if (Mesh && Material)
		SetMeshAndMaterial(Mesh, Material, MeshIndex);
}

StaticMeshComponent::~StaticMeshComponent()
{
	delete _StaticMesh;
	delete _Collision;
}

void StaticMeshComponent::InitialiseComponent(MeshAsset* Mesh, Material* Material)
{
	SetMeshAndMaterial(Mesh, Material);
}

void StaticMeshComponent::Tick(float DeltaTime)
{
	WorldComponent::Tick(DeltaTime);

	if (_Body && _SimulatePhysics)
	{
		btVector3 Vec = _Body->getWorldTransform().getOrigin();

		//Need euler YZX 
		btScalar m[16];
		_Body->getWorldTransform().getOpenGLMatrix(m);
		float fAngZ = atan2f(m[1], m[5]);
		float fAngY = atan2f(m[8], m[10]);
		float fAngX = -asinf(m[9]);

		SetWorldPosition(Vector(Vec.x(), Vec.y(), Vec.z()));
		SetWorldRotation(Rotator::AsDegrees(Rotator(fAngX, fAngZ, fAngY)));
	}

	if (!_SimulatePhysics && _Body)
	{
		MovePhysicsBody(GetWorldTransform());
	}
}

void StaticMeshComponent::SetMeshAndMaterial(MeshAsset* Mesh, Material* Material, int MeshIndex)
{
	CHECK_RETURN(!Mesh, "StaticMeshComponent::SetMeshAndMaterial: Mesh was nullptr");
	CHECK_RETURN(!Material, "StaticMeshComponent::SetMeshAndMaterial: Material was nullptr");

	_StaticMesh = Mesh->GetMesh(MeshIndex);
	_Material = Material;

	RefreshBinds();
}

void StaticMeshComponent::SetStaticMesh(const std::string& MeshName)
{
	//TODO: Change mesh at runtime without creating new object
	MeshAsset* NewAsset = AssetSystem::GetAsset<MeshAsset>(MeshName);
	CHECK_RETURN(!NewAsset, "StaticMeshComponent::SetStaticMesh: Failed to get new static mesh.");

	_StaticMesh = NewAsset->GetMesh(0);

	RefreshBinds(); //TODO: Dont call this twice? dunno
}

void StaticMeshComponent::SetMaterial(Material* NewMaterial)
{
	_Material = NewMaterial;

	RefreshBinds();
}

void StaticMeshComponent::Render()
{
	Renderer::Submit(this);

	if (_DrawOutline)
		DrawOutline();

	WorldComponent::Render();
}

void StaticMeshComponent::RefreshBinds()
{
	VertexLayout MeshLayout;
	_Binds = _StaticMesh->GenerateBinds(MeshLayout);
	_IndexBuffer = _StaticMesh->GetIndexBuffer();

	auto Transform = std::make_shared<TransformConstantBuffer>(this);
	AddBind(Transform);
	_Material->BindMaterial(this, MeshLayout);

	AddBind(Rasterizer::Resolve(false));

	//TODO: Temp stencil testing
	if (_DrawOutline)
	{
		AddBind(std::make_shared<Stencil>(StencilMode::Write));
		VertexBuffer Dummy = VertexBuffer(MeshLayout);
		std::vector<unsigned short> Dummy2;

		OutlineEffect.push_back(BindableVertexBuffer::Resolve(_StaticMesh->_Parent->GetAssetName() + std::to_string(_StaticMesh->_MeshIndex), Dummy));
		OutlineEffect.push_back(IndexBuffer::Resolve(_StaticMesh->_Parent->GetAssetName() + std::to_string(_StaticMesh->_MeshIndex), Dummy2));

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
	Rotator RadianRotation = Rotator::AsRadians(WorldTransform._Rotation);

	Vector Scale = WorldTransform._Scale;
	if (_CurrentlyOutlining)
		Scale += _OutlineThickness;

	auto Trans = DirectX::XMMatrixScaling(Scale.X, Scale.Y, Scale.Z) *
		DirectX::XMMatrixRotationRollPitchYaw(RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll) *
		DirectX::XMMatrixTranslation(WorldTransform._Position.X, WorldTransform._Position.Y, WorldTransform._Position.Z);

	return Trans;
}

void StaticMeshComponent::DrawOutline()
{
	_CurrentlyOutlining = true;
	for (auto& b : OutlineEffect)
	{
		b->Bind();
	}
	RenderCommand::DrawIndexed(GetIndexBuffer().GetCount());
	_CurrentlyOutlining = false;
}

void StaticMeshComponent::DrawComponentDetailsWindow()
{
	WorldComponent::DrawComponentDetailsWindow();

	bool BindsDirty = false;
	BindsDirty |= ImGui::Checkbox("Outline", &_DrawOutline);
	BindsDirty |= ImGui::InputFloat("Outline Scale", &_OutlineThickness, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue);

	if (BindsDirty)
		RefreshBinds();
}

void StaticMeshComponent::GenerateCollision()
{
	delete _Collision;

	btConvexHullShape* Shape = new btConvexHullShape();
	auto Vertices = _StaticMesh->GetCollisionVertices();
	for (auto Vert : Vertices)
	{
		btVector3 btv = btVector3(Vert.X, Vert.Y, Vert.Z);
		Shape->addPoint(btv);
	}

	_Collision = Shape;
	_Collision->setMargin(0.01f);

	Transform trans = GetWorldTransform();
	_Collision->setLocalScaling(btVector3(trans._Scale.X, trans._Scale.Y, trans._Scale.Z));
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
	btScalar bodyMass = _SimulatePhysics ? 20.0f : 0.0f;
	btVector3 bodyInertia;
	_Collision->calculateLocalInertia(bodyMass, bodyInertia);

	//Create the construction info for the body
	btRigidBody::btRigidBodyConstructionInfo bodyCI =
		btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, _Collision, bodyInertia);
	bodyCI.m_restitution = 0.4f;
	bodyCI.m_friction = 0.5f;
	bodyCI.m_rollingFriction = 0.2f;
	bodyCI.m_spinningFriction = 0.3f;

	_Body = new btRigidBody(bodyCI);
	_Body->setUserPointer(this);
}

void StaticMeshComponent::InitialisePhysics()
{
	GenerateCollision();
	CreateRigidBody();
}

btRigidBody* StaticMeshComponent::GetRigidBody()
{
	return _Body;
}

void StaticMeshComponent::SetSimulatePhysics(bool Simulate)
{
	_SimulatePhysics = Simulate;
}

void StaticMeshComponent::MovePhysicsBody(Transform NewTransform)
{
	btMotionState* motionState = _Body->getMotionState();
	btTransform Transform;
	btQuaternion Rotation;
	Rotator RadiansRotation = Rotator::AsRadians(NewTransform._Rotation);
	Rotation.setEuler(RadiansRotation.Yaw, RadiansRotation.Pitch, RadiansRotation.Roll);

	//Set new transform
	Transform.setOrigin(btVector3(NewTransform._Position.X, NewTransform._Position.Y, NewTransform._Position.Z));
	Transform.setRotation(Rotation);

	//Set Scale
	_Collision->setLocalScaling(btVector3(NewTransform._Scale.X, NewTransform._Scale.Y, NewTransform._Scale.Z));

	//Update Transform
	_Body->setWorldTransform(Transform);
	motionState->setWorldTransform(Transform);
	World::GetPhysicsWorld()->updateSingleAabb(_Body);

	//Re-enable physics body
	_Body->activate();
}