#include "pch.h"
#include "Line.h"

#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Shaders\ShaderAsset.h"

#include "Flow/Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Flow/Rendering/Core/Bindables/Shaders/PixelShader.h"

#include "Flow\GameFramework\Components\CameraComponent.h"

#include "Rendering/Core/Bindables/Topology.h"
#include "Rendering/Core/Bindables/VertexBuffer.h"
#include "Rendering/Core/Bindables/InputLayout.h"
#include "Rendering/Core/Vertex/VertexBufferData.h"

VertexLayout								Line::m_VertexLayout = VertexLayout();
std::vector<Bindables::Bindable*>			Line::m_Binds = std::vector<Bindables::Bindable*>();
Bindables::IndexBuffer*						Line::m_IndexBuffer = nullptr;
int											Line::m_Count = 0;

Line::Line()
{
}

void Line::Initialise()
{
	Line* LineObj = new Line();
	LineObj->AddBind(Bindables::Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_LINELIST));

	m_VertexLayout.Append(ElementType::Position3D);

	Array<uint16> indices;
	indices.Add(0);
	indices.Add(1);
	//Bind Index Buffer
	LineObj->AddBind(Bindables::IndexBuffer::Resolve("Line", indices));

	//Material
	LineObj->AddBind(Bindables::PixelShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("LineShader_P")->GetPath()));
	auto vShader = Bindables::VertexShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("LineShader_V")->GetPath());
	auto vShaderByteCode = static_cast<Bindables::VertexShader&>(*vShader).GetByteCode();
	LineObj->AddBind(std::move(vShader));
	LineObj->AddBind(Bindables::InputLayout::Resolve(m_VertexLayout, vShaderByteCode));
}

void Line::DrawLine(Vector3 From, Vector3 To, Vector3 Colour)
{
	Line NewLine = Line();

	// Define Vertex Layout
	VertexBuffer VBuffer(m_VertexLayout);
	VBuffer.EmplaceBack(DirectX::XMFLOAT3{ From.x ,  From.y,  From.z });
	VBuffer.EmplaceBack(DirectX::XMFLOAT3{ To.x ,  To.y,  To.z });

	//Add Vertex Buffer Bind
	Bindables::VertexBuffer BVB = Bindables::VertexBuffer("", VBuffer);

	struct LineColour
	{
		DirectX::XMFLOAT4 Colour;
	} LineColourCBuffer;
	LineColourCBuffer = { { Colour.x, Colour.y, Colour.z, 1.0f } };
	auto ColourBuffer = PixelConstantBuffer<LineColour>(LineColourCBuffer, 3u);


	//Initialise VertexConstBuffer
	struct LineTransform
	{
		DirectX::XMMATRIX ViewProjectionMatrix = DirectX::XMMatrixTranspose(Renderer::GetMainCamera()->GetViewMatrix() * Renderer::GetMainCamera()->GetProjectionMatrix());
	} Trans;

	std::shared_ptr<VertexConstantBuffer<LineTransform>> VCB = std::make_shared<VertexConstantBuffer<LineTransform>>(0);
	VCB->Update(Trans);


	VCB->Bind();
	BVB.Bind();
	ColourBuffer.Bind();


	NewLine.BindAll();

	Renderer::DrawIndexed(m_IndexBuffer->GetCount());

	m_Count++;
}

DirectX::XMMATRIX Line::GetTransformXM() const
{
	return DirectX::XMMATRIX();
}

void Line::AddBind(Bindables::Bindable* bind)
{
	//If index buffer, only allow single bind.
	if (typeid(*bind) == typeid(Bindables::IndexBuffer))
	{
		assert("Renderable::AddBind: Cannot bind multiple index buffers." && m_IndexBuffer == nullptr);
		m_IndexBuffer = static_cast<Bindables::IndexBuffer*>(bind);
	}

	m_Binds.push_back(std::move(bind));
}

void Line::BindAll()
{
	for (auto& b : m_Binds)
	{
		b->Bind();
	}
}