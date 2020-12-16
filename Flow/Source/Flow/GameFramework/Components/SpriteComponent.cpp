#include "Flowpch.h"
#include "SpriteComponent.h"

#include "Rendering/Renderer.h"
#include "Rendering/Core/Vertex/VertexLayout.h"
#include "Rendering/Core/Bindables/ConstantBuffers/TransformConstantBuffer.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ScaledTransformConstantBuffer.h"
#include "Rendering/Core/Bindables/Topology.h"
#include "Rendering/Core/Bindables/BindableVertexBuffer.h"
#include "Rendering/Core/Bindables/IndexBuffer.h"
#include "Rendering/Core/Materials/Material.h"
#include "Rendering/Core/Bindables/Rasterizer.h"

#include "GameFramework/World.h"

#include "Assets/AssetSystem.h"

#include "ThirdParty\ImGui\imgui.h"

SpriteComponent::SpriteComponent()
	: SpriteComponent("Sprite Component")
{
}

SpriteComponent::SpriteComponent(const std::string& Name)
	: RenderableComponent(Name)
	, m_DoubleSided(false)
	, m_Billboard(false)
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
	m_Techniques.clear();

	//= 2D Pass =

	Technique Standard = Technique("SpriteComponent_Standard");
	{
		Step MainStep(0);


		//Topology
		m_Topology = static_cast<Topology*>(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

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

		std::vector<unsigned short> indices;
		indices.reserve(6);
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);

		m_VertexBuffer = static_cast<BindableVertexBuffer*>(BindableVertexBuffer::Resolve("SpriteQuad", VBuffer));
		m_IndexBuffer = static_cast<IndexBuffer*>(IndexBuffer::Resolve("SpriteQuad", indices));

		m_Material->BindMaterial(&MainStep, MeshLayout);

		MainStep.AddBindable(new TransformConstantBuffer(this));

		MainStep.AddBindable(Rasterizer::Resolve(m_DoubleSided ? CullMode::None : CullMode::Back));

		Standard.AddStep(std::move(MainStep));
	}

	AddTechnique(Standard);
}

void SpriteComponent::BeginPlay()
{
	WorldComponent::BeginPlay();

	InitialisePhysics();
}

void SpriteComponent::Tick(float DeltaTime)
{
	//TODO: Do this properly and not in tick
	if (m_Billboard)
	{
		SetWorldRotation(Maths::FindLookAtRotation(GetWorldPosition(), RenderCommand::GetMainCamera()->GetCameraPosition()));
	}
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

	World::Get()->AddPhysicsObject(m_RigidBody);
}

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
	PropertiesChanged |= ImGui::Checkbox("Billboard (Not yet supported properly)", &m_Billboard);

	ImGui::Separator();

	if (PropertiesChanged == true)
	{
		RefreshBinds();
	}
}

std::string SpriteComponent::GetClassSerializationUID(std::ofstream* Archive)
{
	return typeid(SpriteComponent).name();
}

void SpriteComponent::Serialize(std::ofstream* Archive)
{
	WorldComponent::Serialize(Archive);

	//Save Mesh and material
	Archive->write(reinterpret_cast<char*>(&m_DoubleSided), sizeof(bool));
}

void SpriteComponent::Deserialize(std::ifstream* Archive, Actor* NewParent)
{
	WorldComponent::Deserialize(Archive, NewParent);

	Archive->read(reinterpret_cast<char*>(&m_DoubleSided), sizeof(bool));

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
