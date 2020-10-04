#include "Flowpch.h"
#include "DepthBuffer.h"
#include "Flow/Rendering/RenderCommand.h"

DepthBuffer::DepthBuffer(unsigned int Width, unsigned int Height)
{
	Resize(Width, Height);
}

DepthBuffer::~DepthBuffer()
{
	m_DepthTexture->Release();
	m_DepthTextureView->Release();
}

void DepthBuffer::Resize(unsigned int Width, unsigned int Height)
{
	CREATE_RESULT_HANDLE();

	auto Device = RenderCommand::DX11GetDevice();
	// Create the depth stencil texture
	D3D11_TEXTURE2D_DESC DepthDescription = {};
	DepthDescription.Width = Width;
	DepthDescription.Height = Height;
	DepthDescription.MipLevels = 1u;
	DepthDescription.ArraySize = 1u;
	DepthDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //24 bit depth, 8 bit stencil
	DepthDescription.SampleDesc.Count = 1u;
	DepthDescription.SampleDesc.Quality = 0u;
	DepthDescription.Usage = D3D11_USAGE_DEFAULT;
	DepthDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	CATCH_ERROR_DX(Device->CreateTexture2D(&DepthDescription, nullptr, &m_DepthTexture));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDescription = {};
	DepthStencilViewDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDescription.Texture2D.MipSlice = 0u;
	CATCH_ERROR_DX(Device->CreateDepthStencilView(m_DepthTexture.Get(), &DepthStencilViewDescription, m_DepthTextureView.GetAddressOf()));

	m_Width = Width;
	m_Height = Height;
}

ID3D11Texture2D* DepthBuffer::GetTexture() const
{
	return m_DepthTexture.Get();
}

ID3D11DepthStencilView* DepthBuffer::GetTextureView() const
{
	return m_DepthTextureView.Get();
}

unsigned int DepthBuffer::GetWidth() const
{
	return m_Width;
}

unsigned int DepthBuffer::GetHeight() const
{
	return m_Height;
}
