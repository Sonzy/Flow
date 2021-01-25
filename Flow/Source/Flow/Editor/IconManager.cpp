// Includes ///////////////////////////////////////////////////////////////////////

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

// Icon Function Definitions //////////////////////////////////////////////////////

IconManager::IconManager()
	: m_iconVerticesTopLeftAligned(m_iconLayout)
	, m_iconVerticesCentreAligned(m_iconLayout)
	, m_showDebugWindow(false)
	, m_iconSize(50.0f)
{
	m_iconMaterial = AssetSystem::GetAsset<MaterialAsset>("Mat_Texture2D")->GetMaterial();

	// Set up the mesh for icons

	m_iconLayout.Append(ElementType::Position2D);
	m_iconLayout.Append(ElementType::Texture2D);

	//TODO: probs do it in the shader
	m_iconVerticesTopLeftAligned = VertexBuffer(m_iconLayout);
	m_iconVerticesTopLeftAligned.EmplaceBack(DirectX::XMFLOAT2{ 0.0f, 0.0f }, DirectX::XMFLOAT2{ 0.0f, 0.0f });
	m_iconVerticesTopLeftAligned.EmplaceBack(DirectX::XMFLOAT2{ 1.0f, 0.0f }, DirectX::XMFLOAT2{ 1.0f, 0.0f });
	m_iconVerticesTopLeftAligned.EmplaceBack(DirectX::XMFLOAT2{ 1.0f, 1.0f }, DirectX::XMFLOAT2{ 1.0f, 1.0f });
	m_iconVerticesTopLeftAligned.EmplaceBack(DirectX::XMFLOAT2{ 0.0f, 1.0f }, DirectX::XMFLOAT2{ 0.0f, 1.0f });

	m_iconVerticesCentreAligned = VertexBuffer(m_iconLayout);
	m_iconVerticesCentreAligned.EmplaceBack(DirectX::XMFLOAT2{ -0.5f, -0.5f }, DirectX::XMFLOAT2{ 0.0f, 0.0f });
	m_iconVerticesCentreAligned.EmplaceBack(DirectX::XMFLOAT2{  0.5f, -0.5f }, DirectX::XMFLOAT2{ 1.0f, 0.0f });
	m_iconVerticesCentreAligned.EmplaceBack(DirectX::XMFLOAT2{  0.5f,  0.5f }, DirectX::XMFLOAT2{ 1.0f, 1.0f });
	m_iconVerticesCentreAligned.EmplaceBack(DirectX::XMFLOAT2{ -0.5f,  0.5f }, DirectX::XMFLOAT2{ 0.0f, 1.0f });

	m_iconIndices.reserve(6);
	m_iconIndices.push_back(0);
	m_iconIndices.push_back(1);
	m_iconIndices.push_back(2);
	m_iconIndices.push_back(2);
	m_iconIndices.push_back(3);
	m_iconIndices.push_back(0);
}

void IconManager::RegisterIcon(FGUID guid, const IconData& data)
{
	Icon* newIcon = new Icon(guid, data.m_texture);
	newIcon->RefreshBinds(*this);

	m_iconData[guid] = newIcon;
}

void IconManager::RemoveIcon(FGUID guid)
{
	auto iterator = m_iconData.find(guid);
	if (iterator != m_iconData.end())
	{
		m_iconData.erase(iterator);
	}
}

void IconManager::Update()
{
	RenderIcons();
}

void IconManager::Render()
{
	if (m_showDebugWindow && ImGui::Begin("Icon manager"))
	{
		ImGui::InputFloat("Icon Size", &m_iconSize);
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

		//TODO: Remove
		iconData.second->RefreshBinds(*this);

		Renderer::Submit(iconData.second);
	}
}

const Material& IconManager::GetIconMaterial() const
{
	return *m_iconMaterial;
}

const VertexBuffer& IconManager::GetIconVertices(Icon::Alignment alignment) const
{
	switch (alignment)
	{
	case Icon::Alignment::TopLeft:			return m_iconVerticesTopLeftAligned;
	case Icon::Alignment::Centre:			return m_iconVerticesCentreAligned;
	default:								return m_iconVerticesCentreAligned;
	}
}

const std::vector<unsigned short>& IconManager::GetIconIndices() const
{
	return m_iconIndices;
}

const VertexLayout& IconManager::GetIconLayout() const
{
	return m_iconLayout;
}

Icon& IconManager::GetIcon(FGUID iconGuid)
{
	return *m_iconData[iconGuid]; //TODO: what if we get an invalid one lololol do it soon
}

float IconManager::GetIconSize() const
{
	return m_iconSize; //Pixels
}

Icon::Icon(FGUID guid, TextureAsset* tex)
	: m_guid(guid)
	, m_tint(1.0f, 1.0f, 1.0f, 1.0f)
	, m_vCB(new VertexConstantBuffer<IconVertexData>(1)) //TODO: no magic numbers
	, m_pCB(new PixelConstantBuffer<IconPixelData>(5))
{
}

void Icon::RefreshBinds(const IconManager& manager)
{
	PROFILE_FUNCTION();

	//TODO: Batch render these
	m_Techniques.clear();

	Technique Standard = Technique("Basic_Icon");
	{
		//TODO: Fix memory leaks
		Step MainStep(PASS_UI);

		m_Topology = static_cast<Topology*>(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		//Vertex Buffer

		//For now just recreate them in case of a change
		delete m_VertexBuffer;
		m_VertexBuffer = new BindableVertexBuffer("IconQuad", manager.GetIconVertices(m_alignment));

		m_IndexBuffer = static_cast<IndexBuffer*>(IndexBuffer::Resolve("IconQuad", manager.GetIconIndices()));

		manager.GetIconMaterial().BindMaterial(&MainStep, manager.GetIconLayout());
		Standard.AddStep(std::move(MainStep));

		//Update vertex constant buffers

		m_buf.windowSize = RenderCommand::GetWindowSize();
		m_buf.xPosition = m_position.x;
		m_buf.yPosition = m_position.y;
		m_buf.xScale = manager.GetIconSize();
		m_buf.yScale = manager.GetIconSize();

		m_vCB->Update(m_buf);
		m_vCB->Bind();

		//Update pixel constant buffers

		m_pBuf.m_tint = m_tint;

		m_pCB->Update(m_pBuf);
		m_pCB->Bind();
	}

	AddTechnique(Standard);
}

DirectX::XMMATRIX Icon::GetTransformXM() const
{
	return 	DirectX::XMMatrixIdentity();
}
