#pragma once

//= Includes ============================

#include <d3d11.h>
#include <wrl.h>

//= Class Defintion =====================

class DepthBuffer
{
public:

	//= Public Functions =====================================

								DepthBuffer(unsigned int Width, unsigned int Height);
								~DepthBuffer();
	void						Resize(unsigned int Width, unsigned int Height);

	ID3D11Texture2D*			GetTexture() const;
	ID3D11DepthStencilView*		GetTextureView() const;
	unsigned int				GetWidth() const;
	unsigned int				GetHeight() const;

private:

	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_DepthTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_DepthTextureView = nullptr;

	unsigned int									m_Width;
	unsigned int									m_Height;
};