#include "Flowpch.h"
#include "IconManager.h"

#include "Rendering\Renderer.h"

#include "Rendering/Renderer.h"
#include "Rendering/Core/RenderQueue/Pass.h"
#include "Rendering/Core/Vertex/VertexLayout.h"
#include "Rendering/Core/Bindables/ConstantBuffers/TransformConstantBuffer.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ScaledTransformConstantBuffer.h"
#include "Rendering/Core/Bindables/Topology.h"
#include "Rendering/Core/Bindables/BindableVertexBuffer.h"
#include "Rendering/Core/Bindables/IndexBuffer.h"
#include "Rendering/Core/Materials/Material.h"
#include "Rendering/Core/Bindables/Rasterizer.h"

#include "Assets/AssetSystem.h"
#include "Assets/Materials/Mat_Texture2D.h"

#include "GameFramework/World.h"
#include "GameFramework/Components/WorldComponent.h"

#include "ThirdParty/ImGui/imgui.h"

IconManager::IconManager()
{
	m_iconMaterial = AssetSystem::GetAsset<MaterialAsset>("Mat_Texture2D")->GetMaterial();
	//TODO: Figure out material stuff for icons
	//TODO: Delete unused Icons
	m_rotatorOverride = Rotator();
	m_posOverride = Vector3();
}

void IconManager::RegisterIcon(FGUID guid, const IconData& data)
{
	Icon* newIcon = new Icon(guid, data.m_texture);
	newIcon->RefreshBinds(*this);

	m_iconData[guid] = newIcon;

	//newIcon->m_scale = Vector3(100.0f, 50.0f, 1.0f);
	newIcon->m_scale = Vector3(100.0f, 100.0f, 1.0f);
}

void IconManager::RemoveIcon(FGUID guid)
{
	auto iterator = m_iconData.find(guid);
	if (iterator != m_iconData.end())
	{
		m_iconData.erase(iterator);
	}
}

void IconManager::RenderIcons()
{
	PROFILE_FUNCTION();

	const World& world = *World::Get();
	Vector3 cameraPosition = RenderCommand::GetMainCamera()->GetCameraPosition();
	for (auto iconData : m_iconData)
	{
		if (iconData.first == -1)
		{
			FLOW_ENGINE_WARNING("Icon is here for invalid object");
			continue;
		}

		//iconData.second->m_position = world.FindComponent<WorldComponent>(iconData.first)->GetWorldPosition(); //TODO: World to screen
		//iconData.second->m_rotation = Maths::FindLookAtRotation(iconData.second->m_position, cameraPosition);

		iconData.second->m_position = m_posOverride;
		//iconData.second->m_position = RenderCommand::WorldToScreen(world.FindComponent<WorldComponent>(iconData.first)->GetWorldPosition());
		iconData.second->m_rotation = m_rotatorOverride;

		//TODO: REmove
		iconData.second->RefreshBinds(*this);

		Renderer::Submit(iconData.second);
	}
}

const Material& IconManager::GetIconMaterial() const
{
	return *m_iconMaterial;
}

void IconManager::RenderDebugWindow()
{
	if (ImGui::Begin("Icon Manager Debug Window"))
	{
		bool updated = false;
		
		updated |= ImGui::SliderFloat3("Rotation", (float*)&m_rotatorOverride, 0.0f, 360.0f);
		updated |= ImGui::SliderFloat3("Position", (float*)&m_posOverride, -300.0f, 300.0f);
		updated |= ImGui::Checkbox("Toggle InverseViewMatrix", &m_InverseViewMatrix);
		updated |= ImGui::Checkbox("Toggle No View Matrix", &m_NoViewMatrix);

		if (updated)
		{
			for (auto iconData : m_iconData)
			{
				iconData.second->RefreshBinds(*this);
			}
		}
	}
	ImGui::End();
}

// Icon Function Definitions //////////////////////////////////////////////////////


Icon::Icon(FGUID guid, TextureAsset* tex)
	: m_guid(guid)
	, m_texture(tex)
	, m_doubleSided(true)
	, m_scale(1.0f, 1.0f)
{
}

void Icon::RefreshBinds(const IconManager& manager)
{
	//TODO: Batch render these
	m_Techniques.clear();

	Technique Standard = Technique("Basic_Icon");
	{
		Step MainStep(PASS_UI);
		//Step MainStep(PASS_MAIN);

		//Topology
		m_Topology = static_cast<Topology*>(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		//Vertex Layout
		VertexLayout MeshLayout;
		MeshLayout.Append(ElementType::Position2D);
		MeshLayout.Append(ElementType::Texture2D);

		//TODO: Change
		float scale = 1.0f;

		//Vertex Buffer
		VertexBuffer VBuffer(MeshLayout);
		VBuffer.EmplaceBack(DirectX::XMFLOAT2{ m_position.x  + (-0.5f * scale), m_position.y +  (-0.5f * scale)  }, DirectX::XMFLOAT2{ 0.0f, 1.0f });	  //TODO:
		VBuffer.EmplaceBack(DirectX::XMFLOAT2{ m_position.x  + (0.5f  *	scale),  m_position.y + (-0.5f * scale)  }, DirectX::XMFLOAT2{ 1.0f, 1.0f });		  //TODO:
		VBuffer.EmplaceBack(DirectX::XMFLOAT2{ m_position.x  + (0.5f  *	scale),  m_position.y + (0.5f  * scale) }, DirectX::XMFLOAT2{ 1.0f, 0.0f });		  //TODO:
		VBuffer.EmplaceBack(DirectX::XMFLOAT2{ m_position.x  + (-0.5f *	scale), m_position.y +  (0.5f  * scale) }, DirectX::XMFLOAT2{ 0.0f, 0.0f });		  //TODO:

		std::vector<unsigned short> indices;
		indices.reserve(6);
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);

		DirectX::XMFLOAT2 f1 = { m_position.x + -0.5f, m_position.y + -0.5f };
		DirectX::XMFLOAT2 f2 = { m_position.x + 0.5f,  m_position.y + -0.5f };
		DirectX::XMFLOAT2 f3 = { m_position.x + 0.5f,  m_position.y + 0.5f	};
		DirectX::XMFLOAT2 f4 = { m_position.x + -0.5f, m_position.y + 0.5f	};

		//TODO: move to constant buffer
		//m_VertexBuffer = static_cast<BindableVertexBuffer*>(BindableVertexBuffer::Resolve("SpriteQuad", VBuffer));
		m_VertexBuffer = new BindableVertexBuffer("SpriteQuad", VBuffer);
		m_IndexBuffer = static_cast<IndexBuffer*>(IndexBuffer::Resolve("SpriteQuad", indices));

		manager.GetIconMaterial().BindMaterial(&MainStep, MeshLayout);

		//TODO: Remove
		MainStep.AddBindable(new TransformConstantBuffer(this));

		MainStep.AddBindable(Rasterizer::Resolve(m_doubleSided ? CullMode::None : CullMode::Back));

		Standard.AddStep(std::move(MainStep));
	}

	AddTechnique(Standard);
}

DirectX::XMMATRIX Icon::GetTransformXM() const
{
	return 	DirectX::XMMatrixScaling(m_scale.x, m_scale.y, 1.0f) *
		DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
}
