#pragma once

//= Includes ===============================

#include <wrl.h>
#include <d3d11.h>

//= Forward Declarations ===================

class DepthBuffer;

//= Class Definitions ======================

class FrameBuffer
{
public:

	//= Public Functions =======================================

										FrameBuffer(unsigned int Width, unsigned int Height, bool CreateDepthBuffer, DXGI_FORMAT fmt = DXGI_FORMAT_B8G8R8A8_UNORM);
										~FrameBuffer();
	void								Resize(unsigned int Width, unsigned int Height);
	
	ID3D11Texture2D*					GetTexture() const;
	ID3D11ShaderResourceView*			GetTextureView() const;
	std::shared_ptr<DepthBuffer>		GetDepthBuffer() const;
	unsigned int						GetWidth() const;
	unsigned int						GetHeight() const;
	bool								HasDepthBuffer() const;

private:

	//= Private Variables ======================================

	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_Texture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_TextureView = nullptr;
														
	bool												m_HasDepthBuffer;
	std::shared_ptr<DepthBuffer>						m_DepthBuffer;

	DXGI_FORMAT											m_fmt;
														
	unsigned int										m_Width;
	unsigned int										m_Height;
};