#include "pch.h"
#include "SpriteComponent.h"

#include "Rendering/Renderer.h"
#include "Rendering/Core/Vertex/VertexLayout.h"
#include "Rendering/Core/Bindables/ConstantBuffers/TransformConstantBuffer.h"
#include "Rendering/Core/Bindables/Topology.h"
#include "Rendering/Core/Bindables/VertexBuffer.h"
#include "Rendering/Core/Bindables/IndexBuffer.h"
#include "Rendering/Core/Materials/Material.h"
#include "Rendering/Core/Bindables/Rasterizer.h"

#include "GameFramework/World.h"

#include "Assets/AssetSystem.h"

#include "ThirdParty\ImGui\imgui.h"

#include <yaml-cpp/yaml.h>
#include "Framework/Utils/YamlSerializer.h"

SpriteComponent::SpriteComponent()
	: SpriteComponent("Sprite Component")
{
}

SpriteComponent::SpriteComponent(const std::string& Name)
	: RenderableComponent(Name)
	, m_DoubleSided(false)
{
	//TODO: Dont hard code
	m_Material = AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_Green2D")->GetMaterial();

	RefreshBinds();
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::RefreshBinds()
{
	m_techniques.clear();

	//= 2D Pass =

	Technique Standard = Technique("SpriteComponent_Standard");
	{
		Step MainStep(RenderPass::Main);

		//Topology
		m_topology = static_cast<Bindables::Topology*>(Bindables::Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		//Vertex Layout
		VertexLayout MeshLayout;
		MeshLayout.Append(ElementType::Position3D);
		MeshLayout.Append(ElementType::Texture2D);

		//Vertex Buffer
		VertexBuffer VBuffer(MeshLayout);
		VBuffer.EmplaceBack(DirectX::XMFLOAT3{ -0.5f, -0.5f, 0.0f },	DirectX::XMFLOAT2{ 0.0f, 0.0f });
		VBuffer.EmplaceBack(DirectX::XMFLOAT3{ 0.5f, -0.5f, 0.0f },		DirectX::XMFLOAT2{ 1.0f, 0.0f });
		VBuffer.EmplaceBack(DirectX::XMFLOAT3{ 0.5f, 0.5f, 0.0f },		DirectX::XMFLOAT2{ 0.0f, 1.0f });
		VBuffer.EmplaceBack(DirectX::XMFLOAT3{ -0.5f, 0.5f, 0.0f },		DirectX::XMFLOAT2{ 1.0f, 1.0f });			

		Array<uint16> indices;
		indices.Reserve(6);
		indices.Add(0);
		indices.Add(1);
		indices.Add(2);
		indices.Add(2);
		indices.Add(3);
		indices.Add(0);

		m_vertexBuffer = Bindables::VertexBuffer::Resolve("SpriteQuad", VBuffer);
		m_indexBuffer = Bindables::IndexBuffer::Resolve("SpriteQuad", indices);

		m_Material->BindMaterial(&MainStep, MeshLayout);

		MainStep.AddBindable(new TransformConstantBuffer(this));

		MainStep.AddBindable(Rasterizer::Resolve(m_DoubleSided ? Rasterizer::Cull_None : Rasterizer::Cull_Back));

		Standard.AddStep(std::move(MainStep));
	}

	AddTechnique(Standard);
}

void SpriteComponent::BeginPlay()
{
	WorldComponent::BeginPlay();

	InitialisePhysics();
}

void SpriteComponent::Render()
{
	PROFILE_FUNCTION();

	Renderer::Submit(this);

	WorldComponent::Render();
}

void SpriteComponent::InitialisePhysics()
{
	if (m_RigidBody != nullptr)
	{
		World::GetPhysicsWorld()->removeRigidBody(m_RigidBody);
		delete m_RigidBody;
	}

	if (m_CollisionShape != nullptr)
	{
		delete m_CollisionShape;
	}

	GenerateCollision();
	CreateRigidBody();

	if (m_RigidBody == nullptr)
	{
		FLOW_ENGINE_ERROR("SpriteComponent::InitialisePhysics: Rigidbody was nullptr");
		return;
	}

	World::Get().AddPhysicsObject(m_RigidBody);
}

#if WITH_EDITOR
void SpriteComponent::EditorBeginPlay()
{
	WorldComponent::BeginPlay();

	InitialisePhysics();
}

void SpriteComponent::OnViewportSelected()
{
	RenderableComponent::OnViewportSelected();
}

void SpriteComponent::OnViewportDeselected()
{
	RenderableComponent::OnViewportDeselected();
}

void SpriteComponent::DrawComponentDetailsWindow()
{
	WorldComponent::DrawComponentDetailsWindow();

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Sprite Component");

	bool PropertiesChanged = false;

	PropertiesChanged |= ImGui::Checkbox("Double Sided", &m_DoubleSided);

	ImGui::Separator();

	if (PropertiesChanged == true)
	{
		RefreshBinds();
	}
}

#endif //WITH_EDITOR

void SpriteComponent::Serialize(YAML::Emitter& Archive)
{
	WorldComponent::Serialize(Archive);

	Archive << YAML::Key << "SpriteComponent";
	Archive << YAML::BeginMap;
	{
		Archive << YAML::Key << "DoubleSided";
		Archive << YAML::Value << m_DoubleSided;
	}
	Archive << YAML::EndMap;
}

void SpriteComponent::Deserialize(YAML::Node& Archive)
{
	WorldComponent::Deserialize(Archive);

	if (YAML::Node node = Archive["WorldComponent"])
	{
		m_DoubleSided = node["DoubleSided"].as<bool>();
	}

	RefreshBinds();
}

void SpriteComponent::GenerateCollision()
{
	btBoxShape* BoxCollision = new btBoxShape(Vector3(0.5f, 0.5f, 0.01f));

	m_CollisionShape = BoxCollision;
	m_CollisionShape->setMargin(0.01f);

	Transform trans = GetWorldTransform();
	m_CollisionShape->setLocalScaling(btVector3(trans.m_Scale.x, trans.m_Scale.y, trans.m_Scale.z));
}
