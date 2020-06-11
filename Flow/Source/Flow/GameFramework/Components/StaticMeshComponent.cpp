#include "Flowpch.h"
#include "StaticMeshComponent.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"
#include "Flow/Rendering/Core/Bindables/ConstantBuffers/ScaledTransformConstantBuffer.h"

#include "ThirdParty\ImGui\imgui.h"



#include "Flow\GameFramework\World.h"

StaticMeshComponent::StaticMeshComponent()
	: RenderableComponent("Unnamed StaticMesh Component"), _StaticMesh(nullptr), _Material(nullptr)
{
}

StaticMeshComponent::StaticMeshComponent(const std::string& Name, MeshAsset* Mesh, Material* Material, int MeshIndex)
	: RenderableComponent(Name), _StaticMesh(nullptr), _Material(nullptr)
{
	if (Mesh && Material)
		SetMeshAndMaterial(Mesh, Material, MeshIndex);
}

StaticMeshComponent::~StaticMeshComponent()
{
	delete _StaticMesh;
}

void StaticMeshComponent::InitialiseComponent(MeshAsset* Mesh, Material* Material)
{
	SetMeshAndMaterial(Mesh, Material);
}

void StaticMeshComponent::BeginPlay()
{
	WorldComponent::BeginPlay();

#if WITH_EDITOR
	//InitialisePhysics(); //TODO: Editor physics?
#else
	InitialisePhysics();
#endif
}

void StaticMeshComponent::Tick(float DeltaTime)
{
	PROFILE_FUNCTION();

	WorldComponent::Tick(DeltaTime);

}

void StaticMeshComponent::EditorBeginPlay()
{
	WorldComponent::BeginPlay();

	InitialisePhysics();
}

void StaticMeshComponent::OnViewportSelected()
{
	RenderableComponent::OnViewportSelected();

	_Techniques[1].Activate();
}

void StaticMeshComponent::OnViewportDeselected()
{
	RenderableComponent::OnViewportSelected();

	_Techniques[1].Deactivate();
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
	PROFILE_FUNCTION();

	Renderer::Submit(this);

	//if (_DrawOutline)
	//	DrawOutline();

	WorldComponent::Render();
}

void StaticMeshComponent::RefreshBinds()
{
	_Techniques.clear();

	//= Standard Rendering ====
	VertexLayout MeshLayout;
	auto Transform = std::make_shared<TransformConstantBuffer>(this);

	Technique Standard;
	{
		Step MainStep(0);

		//Set the bindables for this specific object (Topology, Indices, VertexBuffer) 
		_StaticMesh->GenerateBinds(MeshLayout);
		_VertexBuffer = _StaticMesh->_BindableVBuffer;
		_IndexBuffer = _StaticMesh->_IndexBuffer;
		_Topology = _StaticMesh->_Topology;

		_Material->BindMaterial(&MainStep, MeshLayout);

		MainStep.AddBindable(std::make_shared<TransformConstantBuffer>(this));

		Standard.AddStep(std::move(MainStep));
	}
	AddTechnique(std::move(Standard));

	Technique Outline;
	Outline.Deactivate();
	{
		Step Masking(3);

		auto VS = VertexShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("SolidColourVS")->GetPath());
		auto VSByteCode = static_cast<VertexShader&>(*VS).GetByteCode();
		Masking.AddBindable(std::move(VS));

		Masking.AddBindable(InputLayout::Resolve(MeshLayout, VSByteCode));
		Masking.AddBindable(std::make_shared<TransformConstantBuffer>(this));

		Outline.AddStep(std::move(Masking));
	}
	{
		Step DrawOutline(4);

		auto VS = VertexShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("SolidColourVS")->GetPath());
		auto VSByteCode = static_cast<VertexShader&>(*VS).GetByteCode();

		DrawOutline.AddBindable(std::move(VS));
		DrawOutline.AddBindable(PixelShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("SolidColourPS")->GetPath()));
		DrawOutline.AddBindable(InputLayout::Resolve(MeshLayout, VSByteCode));
		DrawOutline.AddBindable(std::make_shared<ScaledTransformConstantBuffer>(this));

		Outline.AddStep(DrawOutline);
	}

	AddTechnique(std::move(Outline));
}

DirectX::XMMATRIX StaticMeshComponent::GetTransformXM() const
{
	Transform WorldTransform = GetWorldTransform();
	Rotator RadianRotation = Rotator::AsRadians(WorldTransform._Rotation);
	Vector Scale = WorldTransform._Scale;

	auto Trans = DirectX::XMMatrixScaling(Scale.X, Scale.Y, Scale.Z) *
		DirectX::XMMatrixRotationRollPitchYaw(RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll) *
		DirectX::XMMatrixTranslation(WorldTransform._Position.X, WorldTransform._Position.Y, WorldTransform._Position.Z);

	return Trans;
}

void StaticMeshComponent::DrawComponentDetailsWindow()
{
	WorldComponent::DrawComponentDetailsWindow();

	//TODO: Proper technique probing

	ImGui::Checkbox("Draw Outline", &_Techniques[1].GetWriteAccessToActive());

	bool BindsDirty = false;
	BindsDirty |= ImGui::Checkbox("Outline", &_DrawOutline);
	BindsDirty |= ImGui::InputFloat("Outline Scale", &_OutlineThickness, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue);

	if (BindsDirty)
		RefreshBinds();
}

void StaticMeshComponent::GenerateCollision()
{
	if(_CollisionShape)
		delete _CollisionShape;

	btConvexHullShape* Shape = new btConvexHullShape();
	auto Vertices = _StaticMesh->GetCollisionVertices();
	for (auto Vert : Vertices)
	{
		btVector3 btv = btVector3(Vert.X, Vert.Y, Vert.Z);
		Shape->addPoint(btv);
	}

	_CollisionShape = Shape;
	_CollisionShape->setMargin(0.01f);

	Transform trans = GetWorldTransform();
	_CollisionShape->setLocalScaling(btVector3(trans._Scale.X, trans._Scale.Y, trans._Scale.Z));
}


void StaticMeshComponent::InitialisePhysics()
{
	GenerateCollision();
	CreateRigidBody();

	World* CurrentWorld = World::GetWorld();

	if (!_RigidBody)
	{
		FLOW_ENGINE_ERROR("Tried to add physics object when ");
		return;
	}

	CurrentWorld->AddPhysicsObject(_RigidBody);
}

void StaticMeshComponent::DestroyPhysics()
{
	if (!_RigidBody)
		return;

	World::GetWorld()->GetPhysicsWorld()->removeRigidBody(_RigidBody);
	delete _RigidBody;
}

//void StaticMeshComponent::MovePhysicsBody(Transform NewTransform)
//{
//	PROFILE_FUNCTION();
//
//	btMotionState* motionState = _Body->getMotionState();
//	btTransform Transform;
//	btQuaternion Rotation;
//	Rotator RadiansRotation = Rotator::AsRadians(NewTransform._Rotation);
//	Rotation.setEuler(RadiansRotation.Yaw, RadiansRotation.Pitch, RadiansRotation.Roll);
//
//	//Set new transform
//	Transform.setOrigin(btVector3(NewTransform._Position.X, NewTransform._Position.Y, NewTransform._Position.Z));
//	Transform.setRotation(Rotation);
//
//	//Set Scale
//	_Collision->setLocalScaling(btVector3(NewTransform._Scale.X, NewTransform._Scale.Y, NewTransform._Scale.Z));
//
//	//Update Transform
//	_Body->setWorldTransform(Transform);
//	motionState->setWorldTransform(Transform);
//	World::GetPhysicsWorld()->updateSingleAabb(_Body);
//
//	//Re-enable physics body
//	_Body->activate();
//}

void StaticMeshComponent::SetStencilMode(StencilMode NewMode)
{
	FLOW_ENGINE_WARNING("StaticMeshComponent::SetStencilMode: TODO: Update");
	//_StencilMode = NewMode;

	RefreshBinds();
}
