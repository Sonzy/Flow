#pragma once
#include <string>
#include <DirectXMath.h>

class RenderableComponent;
class VertexLayout;
class ShaderAsset;
class TextureAsset;
class Renderable;
class Step;

/* Base class for materials. Allows for easy setting of textures and shaders. To be extended in the future for dynamic materials */
class FLOW_API Material
{
public:
	Material();

	virtual void BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout);

	void SetTexture(const std::string& TextureName);
	void SetPixelShader(const std::string& ShaderName);
	void SetVertexShader(const std::string& ShaderName);

protected:
	//TODO: Multiple textures etc

	TextureAsset* _Texture;
	ShaderAsset* _VertexShader;
	ShaderAsset* _PixelShader;
};

/* Struct used for sending light data vis */
struct ObjectLightBuffer
{
	DirectX::XMFLOAT3 _SpecularColour;
	float _SpecularWeight;
	float _SpecularGloss;
	float _Padding[3];
};