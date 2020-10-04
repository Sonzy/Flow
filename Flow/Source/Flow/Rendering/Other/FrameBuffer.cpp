#include "Flowpch.h"
#include "FrameBuffer.h"
#include "Flow/Rendering/RenderCommand.h"
#include "DepthBuffer.h"

FrameBuffer::FrameBuffer(unsigned int Width, unsigned int Height, bool CreateDepthBuffer)
	: m_HasDepthBuffer(CreateDepthBuffer)
{
	Resize(Width, Height);
}

FrameBuffer::~FrameBuffer()
{
	m_Texture->Release();
	m_TextureView->Release();
}//

void FrameBuffer::Resize(unsigned int Width, unsigned int Height)
{
	CREATE_RESULT_HANDLE();

	//Create Texture Description
	D3D11_TEXTURE2D_DESC Description = {};
	Description.Width = Width;
	Description.Height = Height;
	Description.MipLevels = 1;
	Description.ArraySize = 1;
	Description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	Description.SampleDesc.Count = 1;
	Description.SampleDesc.Quality = 0;
	Description.Usage = D3D11_USAGE_DEFAULT;
	Description.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	Description.CPUAccessFlags = 0;
	Description.MiscFlags = 0;

	//Create Texture
	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateTexture2D(&Description, nullptr, &m_Texture));

	m_Width = Width;
	m_Height = Height;

	//Create a view to the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = Description.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = -1;
	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateShaderResourceView(m_Texture.Get(), &SRVDesc, &m_TextureView));

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
