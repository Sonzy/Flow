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

#include "Flow/GameFramework/Actor.h"

StaticMeshComponent::StaticMeshComponent()
	: StaticMeshComponent("Unnamed StaticMesh Component")
{
}

StaticMeshComponent::StaticMeshComponent(const std::string& Name, const std::string& MeshName, const std::string& MaterialName, int MeshIndex)
	: RenderableComponent(Name), _StaticMesh(nullptr), _Material(nullptr)
{
	if(!MeshName.empty() && !MaterialName.empty())
		SetMeshAndMaterial(MeshName, MaterialName, MeshIndex);
}

StaticMeshComponent::~StaticMeshComponent()
{
}

void StaticMeshComponent::InitialiseComponent(const std::string& MeshName, const std::string& MaterialName)
{
	SetMeshAndMaterial(MeshName, MaterialName);
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


void StaticMeshComponent::SetMeshAndMaterial(const std::string& MeshName, const std::string& MaterialName, int MeshIndex)
{
	//Set the material
	MaterialAsset* FoundMat = AssetSystem::GetAsset<MaterialAsset>(MaterialName);
	if (!FoundMat)
	{
		FLOW_ENGINE_ERROR("StaticMeshComponent::SetMeshAndMaterial: Failed to find material with name {0}", MaterialName);
		return;
	}
	_Material = FoundMat->GetMaterial();

	//Set the mesh
	MeshAsset* FoundAsset = AssetSystem::GetAsset<MeshAsset>(MeshName);
	if (!FoundAsset)
	{
		_StaticMesh = nullptr;
		_MeshIndex = -1;
		_MeshIdentifier = "None";
		_MaterialIdentifier = "None";
	}
	else
	{
		_StaticMesh = FoundAsset->GetMesh(MeshIndex);
		_MaterialIdentifier = MaterialName;
		_MeshIndex = MeshIndex;
		_MeshIdentifier = MeshName;
	}

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

void StaticMeshComponent::SetMaterial(const std::string& MaterialName)
{
	MaterialAsset* FoundMat = AssetSystem::GetAsset<MaterialAsset>(MaterialName);
	if (!FoundMat)
	{
		FLOW_ENGINE_ERROR("StaticMeshComponent::SetMaterial: Failed to find material with name {0}", MaterialName);
		return;
	}
	_Material = FoundMat->GetMaterial();

	RefreshBinds();
}

void StaticMeshComponent::SetMeshAndMaterial(MeshAsset* NewMesh, MaterialAsset* NewMaterial, int MeshIndex)
{
	//Set the material
	if (!NewMaterial)
	{
		_MaterialIdentifier = "None";
		FLOW_ENGINE_ERROR("StaticMeshComponent::SetMeshAndMaterial: Failed to find material with name {0}", NewMaterial->GetAssetName());
		return;
	}
	_Material = NewMaterial->GetMaterial();
	_MaterialIdentifier = NewMaterial->GetAssetName();

	//Set the mesh
	if (!NewMesh)
	{
		_StaticMesh = nullptr;
		_MeshIndex = -1;
		_MeshIdentifier = "None";
	}
	else
	{
		_StaticMesh = NewMesh->GetMesh(MeshIndex);
		_MeshIndex = MeshIndex;
		_MeshIdentifier = NewMesh->GetAssetName();
	}

	RefreshBinds();
}

void StaticMeshComponent::SetStaticMesh(MeshAsset* NewMesh)
{
	CHECK_RETURN(!NewMesh, "StaticMeshComponent::SetStaticMesh: Failed to get new static mesh.");
	_StaticMesh = NewMesh->GetMesh(0);
	RefreshBinds(); //TODO: Dont call this twice? dunno
}

void StaticMeshComponent::SetMaterial(MaterialAsset* NewMaterial)
{
	if (!NewMaterial)
	{
		FLOW_ENGINE_ERROR("StaticMeshComponent::SetMaterial: Failed to find material with name {0}", NewMaterial->GetAssetName());
		return;
	}
	_Material = NewMaterial->GetMaterial();
	RefreshBinds();
}

std::string StaticMeshComponent::GetClassSerializationUID(std::ofstream* Archive)
{
	return typeid(StaticMeshComponent).name();
}

void StaticMeshComponent::Serialize(std::ofstream* Archive)
{
	WorldComponent::Serialize(Archive);

	//Save Mesh and material
	Archive->write(_MeshIdentifier.c_str(), sizeof(char) * 32);
	Archive->write(_MaterialIdentifier.c_str(), sizeof(char) * 32);
	Archive->write(reinterpret_cast<char*>(&_MeshIndex), sizeof(int));
}

void StaticMeshComponent::Deserialize(std::ifstream* Archive, Actor* NewParent)
{
	WorldComponent::Deserialize(Archive, NewParent);

	//Load Mesh and material
	char MeshName[32];
	char MaterialName[32];
	int MeshIndex;

	Archive->read(MeshName, sizeof(char) * 32);
	Archive->read(MaterialName, sizeof(char) * 32);
	Archive->read(reinterpret_cast<char*>(&MeshIndex), sizeof(int));
	SetMeshAndMaterial(MeshName, MaterialName, MeshIndex);
}

void StaticMeshComponent::Render()
{
	PROFILE_FUNCTION();

	Renderer::Submit(this);

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

	ImGui::Checkbox("Draw Outline", &_Techniques[1].GetWriteAccessToActive());
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

void StaticMeshComponent::SetStencilMode(StencilMode NewMode)
{
	FLOW_ENGINE_WARNING("StaticMeshComponent::SetStencilMode: TODO: Update");
	//_StencilMode = NewMode;

	RefreshBinds();
}
