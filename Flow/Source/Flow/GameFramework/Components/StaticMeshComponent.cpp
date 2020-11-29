#include "Flowpch.h"
#include "StaticMeshComponent.h"
#include "Assets\AssetSystem.h"
#include "Assets\Meshes\MeshAsset.h"
#include "Rendering\Renderer.h"
#include "Rendering\Core\Vertex\VertexLayout.h"
#include "Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ScaledTransformConstantBuffer.h"

#include "ThirdParty\ImGui\imgui.h"
#include "ThirdParty/ImGui/misc/cpp/imgui_stdlib.h"
#include "Utils/ImGuiHelper.h"

#include "GameFramework\World.h"

#include "GameFramework/Actor.h"

#include <algorithm>

StaticMeshComponent::StaticMeshComponent()
	: StaticMeshComponent("Unnamed StaticMesh Component")
{
}

StaticMeshComponent::StaticMeshComponent(const std::string& Name, const std::string& MeshName, const std::string& MaterialName, int MeshIndex)
	: RenderableComponent(Name)
	, m_StaticMesh(nullptr)
	, m_Material(nullptr)
{
	if (!MeshName.empty() && !MaterialName.empty())
	{
		SetMeshAndMaterial(MeshName, MaterialName, MeshIndex);
	}
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

	if (m_Techniques.size() >= 2)
		m_Techniques[1].Activate();
}

void StaticMeshComponent::OnViewportDeselected()
{
	RenderableComponent::OnViewportDeselected();

	//TODO: dont hard code
	if(m_Techniques.size() >= 2)
		m_Techniques[1].Deactivate();
}


void StaticMeshComponent::SetMeshAndMaterial(const std::string& MeshName, const std::string& MaterialName, int MeshIndex)
{
	//Set the material
	MaterialAsset* FoundMat = AssetSystem::GetAsset<MaterialAsset>(MaterialName);
	if (!FoundMat)
	{
		FLOW_ENGINE_ERROR("StaticMeshComponent::SetMeshAndMaterial: Failed to find material with name %s", MaterialName.c_str());
		return;
	}
	m_Material = FoundMat->GetMaterial();

	//Set the mesh
	MeshAsset* FoundAsset = AssetSystem::GetAsset<MeshAsset>(MeshName);
	if (!FoundAsset)
	{
		m_StaticMesh = nullptr;
		m_MeshIndex = -1;
		m_MeshIdentifier = "None";
		m_MaterialIdentifier = "None";
	}
	else
	{
		m_StaticMesh = FoundAsset->GetMesh(MeshIndex);
		m_MaterialIdentifier = MaterialName;
		m_MeshIndex = MeshIndex;
		m_MeshIdentifier = MeshName;
	}

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

void StaticMeshComponent::SetMaterial(const std::string& MaterialName)
{
	MaterialAsset* FoundMat = AssetSystem::GetAsset<MaterialAsset>(MaterialName);
	if (!FoundMat)
	{
		FLOW_ENGINE_ERROR("StaticMeshComponent::SetMaterial: Failed to find material with name %s", MaterialName.c_str());
		return;
	}
	m_Material = FoundMat->GetMaterial();

	RefreshBinds();
}

void StaticMeshComponent::SetMeshAndMaterial(MeshAsset* NewMesh, MaterialAsset* NewMaterial, int MeshIndex)
{
	//Set the material
	if (!NewMaterial)
	{
		m_MaterialIdentifier = "None";
		FLOW_ENGINE_ERROR("StaticMeshComponent::SetMeshAndMaterial: Failed to find material with name %s", NewMaterial->GetAssetName().c_str());
		return;
	}
	m_Material = NewMaterial->GetMaterial();
	m_MaterialIdentifier = NewMaterial->GetAssetName();

	//Set the mesh
	if (!NewMesh)
	{
		m_StaticMesh = nullptr;
		m_MeshIndex = -1;
		m_MeshIdentifier = "None";
	}
	else
	{
		m_StaticMesh = NewMesh->GetMesh(MeshIndex);
		m_MeshIndex = MeshIndex;
		m_MeshIdentifier = NewMesh->GetAssetName();
	}

	RefreshBinds();
}

void StaticMeshComponent::SetStaticMesh(MeshAsset* NewMesh)
{
	CHECK_RETURN(!NewMesh, "StaticMeshComponent::SetStaticMesh: Failed to get new static mesh.");
	m_StaticMesh = NewMesh->GetMesh(0);
	RefreshBinds(); //TODO: Dont call this twice? dunno

	//Update physics
	if (m_CollisionEnabled)
	{
		GenerateCollision();
		if (!m_RigidBody)
		{
			InitialisePhysics();
			return;
		}
		else
		{
			auto* physWorld = World::GetPhysicsWorld();
			physWorld->removeRigidBody(m_RigidBody);
			m_RigidBody->setCollisionShape(m_CollisionShape);
			//physWorld->addRigidBody(m_RigidBody); //TODO?
		}
	}
}

void StaticMeshComponent::SetMaterial(MaterialAsset* NewMaterial)
{
	if (!NewMaterial)
	{
		FLOW_ENGINE_ERROR("StaticMeshComponent::SetMaterial: Failed to find material with name %s", NewMaterial->GetAssetName().c_str());
		return;
	}
	m_Material = NewMaterial->GetMaterial();
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
	Archive->write(m_MeshIdentifier.c_str(), sizeof(char) * 32);
	Archive->write(m_MaterialIdentifier.c_str(), sizeof(char) * 32);
	Archive->write(reinterpret_cast<char*>(&m_MeshIndex), sizeof(int));
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

void StaticMeshComponent::DefaultInitialise()
{
	SetMeshAndMaterial("Box", "Mat_FlatColour_White");

	switch (World::Get()->GetWorldState())
	{
	case WorldState::Editor: EditorBeginPlay(); break;
	case WorldState::Paused:
	case WorldState::InGame: BeginPlay(); break;
	}
}

void StaticMeshComponent::Render()
{
	PROFILE_FUNCTION();

	Renderer::Submit(this);

	WorldComponent::Render();
}

void StaticMeshComponent::RefreshBinds()
{
	m_Techniques.clear();

	//= Standard Rendering ====
	VertexLayout MeshLayout;
	auto Transform = std::make_shared<TransformConstantBuffer>(this);

	if (!m_DrawWithoutDepth)
	{
		Technique Standard = Technique("StaticMeshComponent_Standard");
		{
			Step MainStep(0);

			//Set the bindables for this specific object (Topology, Indices, VertexBuffer) 
			m_StaticMesh->GenerateBinds(MeshLayout);
			m_VertexBuffer = m_StaticMesh->m_BindableVBuffer;
			m_IndexBuffer = m_StaticMesh->m_IndexBuffer;
			m_Topology = m_StaticMesh->m_Topology;

			if (m_Material)
				m_Material->BindMaterial(&MainStep, MeshLayout);

			MainStep.AddBindable(new TransformConstantBuffer(this));

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
			m_StaticMesh->GenerateBinds(MeshLayout);
			m_VertexBuffer = m_StaticMesh->m_BindableVBuffer;
			m_IndexBuffer = m_StaticMesh->m_IndexBuffer;
			m_Topology = m_StaticMesh->m_Topology;

			if (m_Material)
				m_Material->BindMaterial(&MainStep, MeshLayout);

			MainStep.AddBindable(new TransformConstantBuffer(this));

			StandardNoDepthPass.AddStep(std::move(MainStep));
		}
		AddTechnique(StandardNoDepthPass);
	}


	Technique Outline = Technique("StaticMeshComponent_Outline");
	Outline.Deactivate();
	{
		Step Masking(3);

		auto VS = VertexShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("SolidColorVS")->GetPath());
		auto VSByteCode = static_cast<VertexShader&>(*VS).GetByteCode();
		Masking.AddBindable(std::move(VS));

		Masking.AddBindable(InputLayout::Resolve(MeshLayout, VSByteCode));
		Masking.AddBindable(new TransformConstantBuffer(this));

		Outline.AddStep(std::move(Masking));
	}
	{
		Step DrawOutline(4);

		auto VS = VertexShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("SolidColorVS")->GetPath());
		auto VSByteCode = static_cast<VertexShader&>(*VS).GetByteCode();

		DrawOutline.AddBindable(std::move(VS));
		DrawOutline.AddBindable(PixelShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("SolidColorPS")->GetPath()));
		DrawOutline.AddBindable(InputLayout::Resolve(MeshLayout, VSByteCode));
		DrawOutline.AddBindable(new TransformConstantBuffer(this));

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

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Static Mesh Component");

	if (m_Techniques.size() >= 2) //TODO: better system for this
	{
		ImGui::Checkbox("Draw Outline", &m_Techniques[1].GetWriteAccessToActive());
	}

	DrawMeshSelector();
	DrawMaterialSelector();
																									
	ImGui::Checkbox("Draw Without Depth", &m_DrawWithoutDepth);

	ImGui::Separator();
}

void StaticMeshComponent::GenerateCollision()
{
	btConvexHullShape* Shape = new btConvexHullShape();
	auto Vertices = m_StaticMesh->GetCollisionVertices();
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
	if (m_StaticMesh == nullptr)
	{
		FLOW_ENGINE_WARNING("StaticMeshComponent::InitialisePhysics: StaticMesh was nullptr, skipped init");
		return;
	}

	if (m_RigidBody)
	{
		World::GetPhysicsWorld()->removeRigidBody(m_RigidBody);
		delete m_RigidBody;
	}
	if (m_CollisionShape)
	{
		delete m_CollisionShape;
	}

	GenerateCollision();
	CreateRigidBody(); //TODO: Handle non-root component objects

	World* CurrentWorld = World::Get();

	if (!m_RigidBody)
	{
		FLOW_ENGINE_ERROR("StaticMeshComponent::InitialisePhysics: Rigidbody was nullptr");
		return;
	}

	CurrentWorld->AddPhysicsObject(m_RigidBody);

	//FLOW_ENGINE_LOG("Physics initialised for object {0}", m_ObjectName);
}

void StaticMeshComponent::DestroyPhysics()
{
	if (m_RigidBody == nullptr)
	{
		return;
	}

	World::Get()->GetPhysicsWorld()->removeRigidBody(m_RigidBody);
	delete m_RigidBody;
	m_RigidBody = nullptr;
}

void StaticMeshComponent::SetStencilMode(StencilMode NewMode)
{
	FLOW_ENGINE_WARNING("StaticMeshComponent::SetStencilMode: TODO: Update");
	//_StencilMode = NewMode;

	RefreshBinds();
}

void StaticMeshComponent::DrawMeshSelector()
{
	std::vector<const char*> Assets = AssetSystem::BuildAssetList<MeshAsset>();
	static int CurrentItem = 0;
	const char* currentItem = m_MeshIdentifier.c_str();
	if (ImGui::BeginCombo("Mesh Selector", currentItem, 0)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < Assets.size(); n++)
		{
			bool is_selected = (currentItem == Assets[n]);
	
			if (ImGui::Selectable(Assets[n], is_selected))
			{
				currentItem = Assets[n];
	
				m_MeshIdentifier = currentItem;
				SetStaticMesh(currentItem);
				InitialisePhysics();
			}
	
			// Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void StaticMeshComponent::DrawMaterialSelector()
{
	std::vector<const char*> Assets = AssetSystem::BuildAssetList<MaterialAsset>();
	static int CurrentItem = 0;
	const char* currentItem = m_MaterialIdentifier.c_str();
	if (ImGui::BeginCombo("Material Selector", currentItem, 0)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < Assets.size(); n++)
		{
			bool is_selected = (currentItem == Assets[n]);

			if (ImGui::Selectable(Assets[n], is_selected))
			{
				currentItem = Assets[n];

				m_MaterialIdentifier = currentItem;
				SetMaterial(currentItem);
			}

			// Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}
