#include "Flowpch.h"
#include "IconBatcher.h"
#include "Editor/Editor.h"

#include "Rendering/Core/Bindables/Topology.h"
#include "Rendering/Core/Bindables/BindableVertexBuffer.h"
#include "Rendering/Core/Bindables/IndexBuffer.h"
#include "Rendering/Core/Bindables/Rasterizer.h"
#include "Rendering/Core/Bindables/ConstantBuffers/TransformConstantBuffer.h"
#include "Rendering/Core/Vertex/VertexLayout.h"
#include "Rendering/Core/Vertex/VertexBuffer.h"
#include "Rendering/Core/RenderQueue/RenderQueue.h"
#include "Rendering/Renderer.h"
#include "Assets/AssetSystem.h"
#include "Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Rendering/Core/Bindables/InputLayout.h"
#include "Rendering/Core/Bindables/Texture.h"
#include "Rendering/Core/Bindables/Sampler.h"

#include <d3d11.h>
#include "Maths/DirectXConvertors.h"

IconBatcher::IconBatcher()
{
}

void IconBatcher::RenderIcons()
{
	return;

	//Topology*				IconTopology = nullptr;
	//BindableVertexBuffer*	IconVertexBuffer = nullptr;
	//IndexBuffer*			IconIndexBuffer = nullptr;
	//PixelShader*			IconPixelShader = nullptr;
	//VertexShader*			IconVertexShader = nullptr;
	//Sampler*				IconSampler = nullptr;
	//InputLayout*			IconInputLayout = nullptr;
	//
	////Setup static bindables
	//	
	//IconTopology = Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//
	//VertexLayout MeshLayout;
	//MeshLayout.Append(ElementType::Position2D);
	//MeshLayout.Append(ElementType::Texture2D);
	//
	//VertexBuffer VBuffer(MeshLayout);
	//VBuffer.EmplaceBack(DirectX::XMFLOAT2{ -0.5f, -0.5f },	DirectX::XMFLOAT2{ 0.0f, 0.0f });
	//VBuffer.EmplaceBack(DirectX::XMFLOAT2{  0.5f, -0.5f	},	DirectX::XMFLOAT2{ 1.0f, 0.0f });
	//VBuffer.EmplaceBack(DirectX::XMFLOAT2{  0.5f,  0.5f	},	DirectX::XMFLOAT2{ 0.0f, 1.0f });
	//VBuffer.EmplaceBack(DirectX::XMFLOAT2{ -0.5f,  0.5f	},	DirectX::XMFLOAT2{ 1.0f, 1.0f });
	//
	//std::vector<unsigned short> indices;
	//indices.reserve(6);
	//indices.push_back(0);
	//indices.push_back(1);
	//indices.push_back(2);
	//indices.push_back(2);
	//indices.push_back(3);
	//indices.push_back(0);
	//
	//IconVertexBuffer = BindableVertexBuffer::Resolve("SpriteQuad", VBuffer);
	//IconIndexBuffer = IndexBuffer::Resolve("SpriteQuad", indices);
	//
	//IconSampler = Sampler::Resolve();
	//
	//// Shaders
	//
	//ShaderAsset* VertexShaderAsset = AssetSystem::GetAsset<ShaderAsset>("SolidColor2D_VS");
	//ShaderAsset* PixelShaderAsset = AssetSystem::GetAsset<ShaderAsset>("SolidColor2D_PS");
	//IconVertexShader = VertexShader::Resolve(VertexShaderAsset->GetPath());
	//auto vShaderByteCode = static_cast<VertexShader&>(*IconVertexShader).GetByteCode();
	//IconPixelShader = PixelShader::Resolve(PixelShaderAsset->GetPath());
	//IconInputLayout = InputLayout::Resolve(MeshLayout, vShaderByteCode);
	//
	//// Bind them
	//IconTopology->Bind();
	//IconVertexBuffer->Bind();
	//IconIndexBuffer->Bind();
	//IconPixelShader->Bind();
	//IconVertexShader->Bind();
	//IconSampler->Bind();
	//IconInputLayout->Bind();
	//
	//
	//
	////TODO: Put constant buffer stuff in
	//struct IconConstantBuffer
	//{	
	//	DirectX::XMMATRIX WorldViewProj;
	//	DirectX::XMFLOAT2	m_Position;
	//	DirectX::XMFLOAT4	m_Tint;
	//} IconBuffer;
	//
	//VertexConstantBuffer<IconConstantBuffer>* IconVertexConstantBuffer = VertexConstantBuffer<IconConstantBuffer>::Resolve(IconBuffer, 2u, "IconCustomConstantBuffer");
	//
	////TODO: Orthographic
	//RenderCommand::GetMainCamera()->SetProjectionMatrix(DirectX::XMMatrixOrthographicLH(
	//	RenderCommand::GetWindowSize().x,
	//	RenderCommand::GetWindowSize().y,
	//	RenderCommand::GetNearPlane(),
	//	RenderCommand::GetFarPlane()
	//));
	//
	//for (IconData& data : m_IconData)
	//{
	//	IconBuffer.m_Position = DirectX::Convert(data.m_Position);
	//	IconBuffer.m_Tint = DirectX::Convert(data.m_Tint);
	//
	//	IconBuffer.WorldViewProj =
	//		DirectX::XMMatrixTranslation(data.m_Position.x, data.m_Position.y, 0.0f) *
	//		RenderCommand::GetMainCamera()->GetViewMatrix();
	//
	//	//TODO: Update constant buffer data for each item
	//	//Update constant buffer each turn
	//	IconVertexConstantBuffer->Update(IconBuffer);
	//	IconVertexConstantBuffer->Bind();
	//	RenderCommand::DrawIndexed(IconIndexBuffer->GetCount());
	//}
	//
	////TODO: Perspective
	//RenderCommand::GetMainCamera()->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(
	//	Maths::DegreesToRadians(RenderCommand::GetMainCamera()->GetFOV()),
	//	(float)RenderCommand::GetWindowSize().x / (float)RenderCommand::GetWindowSize().y,
	//	RenderCommand::GetNearPlane(),
	//	RenderCommand::GetFarPlane()
	//));
}

void IconBatcher::PushIcon(IconData newData)
{
	m_IconData.push_back(std::move(newData));
}
