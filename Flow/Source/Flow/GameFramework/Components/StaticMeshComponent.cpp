#include "Flowpch.h"
#include "StaticMeshComponent.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"
#include "Flow/Rendering/Core/Bindables/ConstantBuffers/ScaledTransformConstantBuffer.h"

#include "ThirdParty\ImGui\imgui.h"
#include "ThirdParty/ImGui/misc/cpp/imgui_stdlib.h"
#include "Flow/Utils/ImGuiHelper.h"

#include "Flow\GameFramework\World.h"

#include "Flow/GameFramework/Actor.h"

#include <algorithm>

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

	if (_Techniques.size() >= 2)
		_Techniques[1].Activate();
}

void StaticMeshComponent::OnViewportDeselected()
{
	RenderableComponent::OnViewportSelected();

	//TODO: dont hard code
	if(_Techniques.size() >= 2)
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

	//Update physics
	if (m_CollisionEnabled)
	{
		GenerateCollision();
		if (!_RigidBody)
		{
			InitialisePhysics();
			return;
		}
		else
		{
			auto* physWorld = World::GetPhysicsWorld();
			physWorld->removeRigidBody(_RigidBody);
			_RigidBody->setCollisionShape(m_CollisionShape);
			//physWorld->addRigidBody(_RigidBody); //TODO?
		}
	}
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

	if (!m_DrawWithoutDepth)
	{
		Technique Standard = Technique("StaticMeshComponent_Standard");
		{
			Step MainStep(0);

			//Set the bindables for this specific object (Topology, Indices, VertexBuffer) 
			_StaticMesh->GenerateBinds(MeshLayout);
			_VertexBuffer = _StaticMesh->m_BindableVBuffer;
			_IndexBuffer = _StaticMesh->m_IndexBuffer;
			_Topology = _StaticMesh->m_Topology;

			if (_Material)
				_Material->BindMaterial(&MainStep, MeshLayout);

			MainStep.AddBindable(std::make_shared<TransformConstantBuffer>(this));

			Standard.AddStep(std::move(MainStep));
		}
		AddTechnique(Standard);
	}
	else
	{
		Technique StandardNoDepthPass = Technique("StaticMeshComponent_StandardNoDepthPass");
		{
			Step MainStep(5);

			//Set the bindables for this specific object (Topology, Indices, VertexBuffer) 
			_StaticMesh->GenerateBinds(MeshLayout);
			_VertexBuffer = _StaticMesh->m_BindableVBuffer;
			_IndexBuffer = _StaticMesh->m_IndexBuffer;
			_Topology = _StaticMesh->m_Topology;

			if (_Material)
				_Material->BindMaterial(&MainStep, MeshLayout);

			MainStep.AddBindable(std::make_shared<TransformConstantBuffer>(this));

			StandardNoDepthPass.AddStep(std::move(MainStep));
		}
		AddTechnique(StandardNoDepthPass);
	}


	Technique Outline = Technique("StaticMeshComponent_Outline");
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

	AddTechnique(Outline);
}

DirectX::XMMATRIX StaticMeshComponent::GetTransformXM() const
{
	Transform WorldTransform = GetWorldTransform();
	Rotator RadianRotation = Rotator::AsRadians(WorldTransform.m_Rotation);
	Vector3 Scale = WorldTransform.m_Scale;

	auto Trans = DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll) *
		DirectX::XMMatrixTranslation(WorldTransform.m_Position.x, WorldTransform.m_Position.y, WorldTransform.m_Position.z);

	return Trans;
}

void StaticMeshComponent::DrawComponentDetailsWindow()
{
	WorldComponent::DrawComponentDetailsWindow();

	if(_Techniques.size() >= 2) //TODO: better system for this
		ImGui::Checkbox("Draw Outline", &_Techniques[1].GetWriteAccessToActive());

	std::string MeshNameTemp = _MeshIdentifier;
	std::string MatNameTemp = _MaterialIdentifier;

	if (_MeshIdentifier.empty())	_MeshIdentifier = "None";
	if (_MaterialIdentifier.empty())	_MaterialIdentifier = "None";

	if (ImGui::InputText("Mesh Identifier", &MeshNameTemp, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		//Format the string correctly.
		std::string::iterator it = std::remove(MeshNameTemp.begin(), MeshNameTemp.end(), ' ');
		MeshNameTemp.erase(it, MeshNameTemp.end());

		MeshAsset* FoundMesh = AssetSystem::GetAsset<MeshAsset>(MeshNameTemp);
		if (!FoundMesh)
		{
			FLOW_ENGINE_ERROR("StaticMeshComponent::DrawComponentDetailsWindow: Failed to find mesh with name {0}.", MeshNameTemp);
			return;
		}

		_MeshIdentifier = MeshNameTemp;
		SetStaticMesh(FoundMesh);
		InitialisePhysics();
	}
																									//Doesnt allow us to add it, is added automatically?
	if (ImGui::InputText("Material Identifier", &MatNameTemp, ImGuiInputTextFlags_EnterReturnsTrue /*| ImGuiInputTextFlags_CallbackResize */ , ImHelp::InputTextCallback))
	{
		MaterialAsset* FoundMat = AssetSystem::GetAsset<MaterialAsset>(MatNameTemp);
		if (!FoundMat)
		{
			FLOW_ENGINE_ERROR("StaticMeshComponent::DrawComponentDetailsWindow: Failed to find material with name {0}", MatNameTemp);
			return;
		}

		_MaterialIdentifier = MatNameTemp;
		SetMaterial(FoundMat);
	}

	ImGui::Checkbox("Draw Without Depth", &m_DrawWithoutDepth);
}

void StaticMeshComponent::GenerateCollision()
{
	btConvexHullShape* Shape = new btConvexHullShape();
	auto Vertices = _StaticMesh->GetCollisionVertices();
	for (auto Vert : Vertices)
	{
		Shape->addPoint(Vert);
	}

	m_CollisionShape = Shape;
	m_CollisionShape->setMargin(0.01f);

	Transform trans = GetWorldTransform();
	m_CollisionShape->setLocalScaling(btVector3(trans.m_Scale.x, trans.m_Scale.y, trans.m_Scale.z));
}


void StaticMeshComponent::InitialisePhysics()
{
	if (!_StaticMesh)
	{
		FLOW_ENGINE_WARNING("StaticMeshComponent::InitialisePhysics: StaticMesh was nullptr, skipped init");
		return;
	}

	if (_RigidBody)
	{
		World::GetPhysicsWorld()->removeRigidBody(_RigidBody);
		delete _RigidBody;
	}
	if (m_CollisionShape)
	{
		delete m_CollisionShape;
	}

	GenerateCollision();
	CreateRigidBody();

	World* CurrentWorld = World::Get();

	if (!_RigidBody)
	{
		FLOW_ENGINE_ERROR("StaticMeshComponent::InitialisePhysics: Rigidbody was nullptr");
		return;
	}

	CurrentWorld->AddPhysicsObject(_RigidBody);
}

void StaticMeshComponent::DestroyPhysics()
{
	if (!_RigidBody)
		return;

	World::Get()->GetPhysicsWorld()->removeRigidBody(_RigidBody);
	delete _RigidBody;
}

void StaticMeshComponent::SetStencilMode(StencilMode NewMode)
{
	FLOW_ENGINE_WARNING("StaticMeshComponent::SetStencilMode: TODO: Update");
	//_StencilMode = NewMode;

	RefreshBinds();
}
