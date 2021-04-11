//= Includes ========================================================

#include "pch.h"
#include "LineBatcher.h"

#include "Rendering/Core/Bindables/Topology.h"
#include "Rendering/Core/Bindables/InputLayout.h"
#include "Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Rendering/Core/Bindables/VertexBuffer.h"
#include "Rendering/Core/Bindables/Stencil.h"
#include "Rendering/Core/Vertex/VertexLayout.h"
#include "Rendering/Core/Vertex/VertexBufferData.h"

#include "Rendering/Core/Camera/Camera.h"

#include "Assets/AssetSystem.h"

//= Class (LineBatcher) Definition ==================================

LineBatcher::LineBatcher()
	: m_VertexBuffer(nullptr)
	, m_BindableVertexBuffer(nullptr)
	, m_VertexLayout(nullptr)
	, m_VertexCB(nullptr)
{

}

LineBatcher::~LineBatcher()
{
	delete m_VertexBuffer;
	delete m_VertexCB;
	delete m_VertexLayout;
	//delete _BindableVertexBuffer; TODO: Crashes on release?
}

void LineBatcher::Initialise()
{
	AddBind(Bindables::Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_LINELIST));

	m_VertexLayout = new VertexLayout();
	m_VertexLayout->Append(ElementType::Position3D);
	m_VertexLayout->Append(ElementType::Float3Colour);

	m_VertexBuffer = new VertexBuffer(*m_VertexLayout);

	AddBind(Bindables::PixelShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("LineColor_PS")->GetPath()));
	auto vShader = Bindables::VertexShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("LineColor_VS")->GetPath());
	auto vShaderByteCode = static_cast<Bindables::VertexShader&>(*vShader).GetByteCode();
	AddBind(std::move(vShader));
	AddBind(Bindables::InputLayout::Resolve(*m_VertexLayout, vShaderByteCode));
	AddBind(Bindables::Stencil::Resolve(Bindables::Stencil::Mode::AlwaysOnTop));

	m_VertexCB = new VertexConstantBuffer<ViewProjectionBuffer>(0);
}

void LineBatcher::AddLine(Vector3 From, Vector3 To, Vector3 Colour)
{
	m_VertexBuffer->EmplaceBack(
		DirectX::XMFLOAT3{ From.x, From.y, From.z },
		DirectX::XMFLOAT3{ Colour.x, Colour.y, Colour.z });

	m_VertexBuffer->EmplaceBack(
		DirectX::XMFLOAT3{ To.x, To.y, To.z },
		DirectX::XMFLOAT3{ Colour.x, Colour.y, Colour.z });

	m_Lines++;
}

void LineBatcher::AddPersistentLine(const std::string& tag, Vector3 From, Vector3 To, Vector3 Colour)
{
	m_PersistentLines[tag] = Line{ From, To, Colour };
}

void LineBatcher::DrawLines()
{
	if (m_Lines == 0 && m_PersistentLines.size() == 0)
	{
		return;
	}

	for (const std::pair<std::string, Line>& line : m_PersistentLines)
	{
		AddLine(line.second.From, line.second.To, line.second.Color);
	}

	m_BindableVertexBuffer = new Bindables::VertexBuffer("LineBatcher", *m_VertexBuffer);

	m_CameraMatrix._ViewProjectionMatrix = Renderer::GetMainCamera()->GetTransposedCachedViewProjection();
	m_VertexCB->Update(m_CameraMatrix);

	m_VertexCB->Bind();
	m_BindableVertexBuffer->Bind();
	BindAll();


	Renderer::Draw(m_Lines * 2);

	FlushLines();
}

void LineBatcher::FlushLines()
{
	delete m_VertexBuffer;
	delete m_BindableVertexBuffer;

	m_VertexBuffer = new VertexBuffer(*m_VertexLayout);
	m_Lines = 0;
}

void LineBatcher::BindAll()
{
	for (const auto& Bind : m_Binds)
	{
		Bind->Bind();
	}
}

void LineBatcher::AddBind(Bindables::Bindable* NewBind)
{
	m_Binds.push_back(std::move(NewBind));
}
