#pragma once
#include <d3d11.h>
#include <wrl.h>

class DepthBuffer
{
public:

	DepthBuffer(unsigned int Width, unsigned int Height);
	~DepthBuffer();
	void Resize(unsigned int Width, unsigned int Height);

	ID3D11Texture2D* GetTexture() const;
	ID3D11DepthStencilView* GetTextureView() const;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

private:

	Microsoft::WRL::ComPtr<ID3D11Texture2D> _DepthTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _DepthTextureView = nullptr;

	unsigned int _Width;
	unsigned int _Height;
};