// Includes ///////////////////////////////////////////////////////////////////////

#include "pch.h"

#if WITH_EDITOR

#include "IconManager.h"

#include "Rendering\Renderer.h"

#include "Rendering/Renderer.h"
#include "Rendering/Core/RenderQueue/Pass.h"
#include "Rendering/Core/Vertex/VertexLayout.h"
#include "Rendering/Core/Camera/Camera.h"
#include "Rendering/Core/Bindables/ConstantBuffers/TransformConstantBuffer.h"
#include "Rendering/Core/Bindables/Topology.h"
#include "Rendering/Core/Bindables/VertexBuffer.h"
#include "Rendering/Core/Bindables/IndexBuffer.h"
#include "Rendering/Core/Materials/Material.h"
#include "Rendering/Core/Bindables/Rasterizer.h"
#include "Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Rendering/Core/Bindables/Shaders/VertexShader.h"

#include "Assets/AssetSystem.h"
#include "Assets/Materials/Mat_Texture2D.h"

#include "GameFramework/World.h"
#include "GameFramework/Components/WorldComponent.h"

#include "ThirdParty/ImGui/imgui.h"

Material* IconManager::sm_iconMaterialDefault = nullptr;

// Icon Function Definitions //////////////////////////////////////////////////////

IconManager::IconManager()
	: m_iconVerticesTopLeftAligned(m_iconLayout)
	, m_iconVerticesCentreAligned(m_iconLayout)
	, m_showDebugWindow(false)
	, m_iconSize(50.0f)
{
	sm_iconMaterialDefault = AssetSystem::GetAsset<MaterialAsset>("Mat_Texture2D")->GetMaterial();

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

	m_iconIndices.Reserve(6);
	m_iconIndices.Add(0);
	m_iconIndices.Add(1);
	m_iconIndices.Add(2);
	m_iconIndices.Add(2);
	m_iconIndices.Add(3);
	m_iconIndices.Add(0);
}

void IconManager::RegisterIcon(FGuid guid, const IconData& data)
{
	if (m_iconData.find(guid) != m_iconData.end())
	{
		//TODO: will want to support multiple icons in tehf uture
		FLOW_ENGINE_WARNING("IconManager::RegisterIcon: Tried to register an icon when one already exists. %lu", guid);
	}

	Icon* newIcon = new Icon(guid, data.m_texture);
	newIcon->RefreshBinds(*this);

	m_iconData[guid] = newIcon;
}

void IconManager::RemoveIcon(FGuid guid)
{
	auto iterator = m_iconData.find(guid);
	if (iterator != m_iconData.end())
	{
		m_iconData.erase(iterator);
	}
}

void IconManager::Reset()
{
	m_iconData.clear();
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

	const World& world = World::Get();
	Vector3 cameraPosition = Renderer::GetMainCamera()->GetCameraPosition();
	for (auto iconData : m_iconData)
	{
		if (iconData.first == -1)
		{
			FLOW_ENGINE_WARNING("Icon is here for invalid object");
			continue;
		}
				
		if (WorldComponent* parent = World::Get().FindComponent<WorldComponent>(iconData.first))
		{
			parent->IconUpdate(*this);
			Renderer::Submit(iconData.second);
		}
		else
		{
			FLOW_ENGINE_WARNING("Icon parent is nullptr");
		}
	}
}

const Material* IconManager::GetIconMaterial()
{
	return sm_iconMaterialDefault;
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

const Array<uint16>& IconManager::GetIconIndices() const
{
	return m_iconIndices;
}

const VertexLayout& IconManager::GetIconLayout() const
{
	return m_iconLayout;
}

Icon& IconManager::GetIcon(FGuid iconGuid)
{
	return *m_iconData[iconGuid]; //TODO: what if we get an invalid one lololol do it soon
}

float IconManager::GetIconSize() const
{
	return m_iconSize; //Pixels
}

Icon::Icon(FGuid guid, TextureAsset* tex)
	: m_guid(guid)
	, m_tint(1.0f, 1.0f, 1.0f, 1.0f)
	, m_vCB(new VertexConstantBuffer<IconVertexData>(1)) //TODO: no magic numbers
	, m_pCB(new PixelConstantBuffer<IconPixelData>(0))
{
	m_iconMaterial = *IconManager::GetIconMaterial();
	m_iconMaterial.SetTexture(tex->GetAssetName());
}

void Icon::RefreshBinds(const IconManager& manager)
{
	PROFILE_FUNCTION();

	//TODO: Batch render these
	m_techniques.clear();

	Technique Standard = Technique("Basic_Icon");
	{
		//TODO: Fix memory leaks
		Step MainStep(RenderPass::Standard2D);

		m_topology = static_cast<Bindables::Topology*>(Bindables::Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		delete m_vertexBuffer;	//For now just recreate them in case of a change
		m_vertexBuffer = new Bindables::VertexBuffer(HashString((std::string("IconQuad_") + std::to_string((int)m_alignment)).c_str()), manager.GetIconVertices(m_alignment)); //TODO: Manage the tags
		m_indexBuffer = static_cast<Bindables::IndexBuffer*>(Bindables::IndexBuffer::Resolve((std::string("IconQuad_") + std::to_string((int)m_alignment)).c_str(), manager.GetIconIndices()));

		m_iconMaterial.BindMaterial(&MainStep, manager.GetIconLayout());

		//Update vertex constant buffers
		m_buf.windowSize = Renderer::GetWindowSize();
		m_buf.xPosition = m_position.x;
		m_buf.yPosition = m_position.y;
		m_buf.xScale = manager.GetIconSize();
		m_buf.yScale = manager.GetIconSize();
		m_vCB->Update(m_buf);
		MainStep.AddBindable(m_vCB);

		//Update pixel constant buffers
		m_pBuf.m_tint = m_tint;
		m_pCB->Update(m_pBuf);
		MainStep.AddBindable(m_pCB);

		Standard.AddStep(std::move(MainStep));
	}
	AddTechnique(Standard);


	Technique Selection = Technique("Icon_Selection2D");
	{
		SelectionPassConstantBuffer buff
		{
			Vector4(
			(float)(m_guid & 0xff000000) / 4278190080.0f,
			(float)(m_guid & 0x00ff0000) / 16711680.0f,
			(float)(m_guid & 0x0000ff00) / 65280.0f,
			(float)(m_guid & 0x000000ff) / 255.0f)
		};
	
		Step Rendering(RenderPass::Selection2D);

		//No need to recreate mesh binds
		std::string tag = "SelectionBuffer_" + std::to_string(m_guid);
		Rendering.AddBindable(PixelConstantBuffer<SelectionPassConstantBuffer>::Resolve(buff, MaterialCommon::Register::Selection, tag.c_str()));

		auto vShader = Bindables::VertexShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("Selection2D_VS")->GetPath());
		auto vShaderByteCode = static_cast<Bindables::VertexShader&>(*vShader).GetByteCode();
		Rendering.AddBindable(std::move(vShader));
		Rendering.AddBindable(Bindables::PixelShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("Selection2D_PS")->GetPath()));

		m_buf.windowSize = Renderer::GetWindowSize();
		m_buf.xPosition = m_position.x;
		m_buf.yPosition = m_position.y;
		m_buf.xScale = manager.GetIconSize();
		m_buf.yScale = manager.GetIconSize();
		m_vCB->Update(m_buf);
		Rendering.AddBindable(m_vCB);

		Selection.AddStep(std::move(Rendering));

		AddTechnique(Selection);
	}
}

DirectX::XMMATRIX Icon::GetTransformXM() const
{
	return 	DirectX::XMMatrixIdentity();
}

#endif //WITH_EDITOR