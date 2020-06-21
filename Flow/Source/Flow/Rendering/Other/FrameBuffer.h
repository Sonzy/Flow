#pragma once
#include <wrl.h>
#include <d3d11.h>

class FrameBuffer
{
public:

	FrameBuffer(unsigned int Width, unsigned int Height);
	~FrameBuffer();
	void Resize(unsigned int Width, unsigned int Height);
	
	ID3D11Texture2D* GetTexture() const;
	ID3D11ShaderResourceView* GetTextureView() const;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

protected:

	Microsoft::WRL::ComPtr<ID3D11Texture2D> _Texture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _TextureView = nullptr;

	unsigned int _Width;
	unsigned int _Height;
};