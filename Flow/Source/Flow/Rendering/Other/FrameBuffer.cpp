#include "pch.h"
#include "FrameBuffer.h"
#include "Flow/Rendering/Renderer.h"
#include "DepthBuffer.h"
#include "Framework/Utils/DirectX11/DirectX11Utils.h"

FrameBuffer::FrameBuffer(const char* bufferName, unsigned int Width, unsigned int Height, bool CreateDepthBuffer, DXGI_FORMAT fmt)
	: m_HasDepthBuffer(CreateDepthBuffer)
	, m_Texture(nullptr)
	, m_TextureView(nullptr)
	, m_fmt(fmt)
	, m_bufferName(bufferName)
{
	Resize(Width, Height);
}

FrameBuffer::~FrameBuffer()
{
	m_Texture.Reset();
	m_TextureView.Reset();
}//

void FrameBuffer::Resize(unsigned int Width, unsigned int Height)
{
	CreateResultHandle();

	//Create Texture Description
	D3D11_TEXTURE2D_DESC Description = {};
	Description.Width = Width;
	Description.Height = Height;
	Description.MipLevels = 1;
	Description.ArraySize = 1;
	Description.Format = m_fmt;
	Description.SampleDesc.Count = 1;
	Description.SampleDesc.Quality = 0;
	Description.Usage = D3D11_USAGE_DEFAULT;
	Description.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	Description.CPUAccessFlags = 0;
	Description.MiscFlags = 0;

	//Create Texture
	CaptureDXError(Renderer::GetDevice()->CreateTexture2D(&Description, nullptr, &m_Texture));

	m_Width = Width;
	m_Height = Height;

	//Create a view to the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = Description.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = -1;
	CaptureDXError(Renderer::GetDevice()->CreateShaderResourceView(m_Texture.Get(), &SRVDesc, &m_TextureView));

	if (m_HasDepthBuffer)
	{
		if (!m_DepthBuffer)
			m_DepthBuffer = std::make_shared<DepthBuffer>(Width, Height);
		else
			m_DepthBuffer->Resize(Width, Height);
	}
}

ID3D11Texture2D* FrameBuffer::GetTexture() const
{
	return m_Texture.Get();
}

ID3D11ShaderResourceView* FrameBuffer::GetTextureView() const
{
	return m_TextureView.Get();
}

std::shared_ptr<DepthBuffer> FrameBuffer::GetDepthBuffer() const
{
	return m_DepthBuffer;
}

unsigned int FrameBuffer::GetWidth() const
{
	return m_Width;
}

unsigned int FrameBuffer::GetHeight() const
{
	return m_Height;
}

bool FrameBuffer::HasDepthBuffer() const
{
	return m_HasDepthBuffer;
}
