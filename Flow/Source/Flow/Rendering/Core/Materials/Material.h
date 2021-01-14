#pragma once

//= Includes ===================================

#include <DirectXMath.h>
#include <string>

//= Forward Declarations =======================

class TextureAsset;
class ShaderAsset;
class Step;
class Renderable;
class RenderableComponent;
class VertexLayout;

//= Global Struct Definitions ==================

/* Struct used for sending light data vis */
struct ObjectLightBuffer
{
	DirectX::XMFLOAT3					m_SpecularColour;
	float								m_SpecularWeight;
	float								m_SpecularGloss;
	float								m_Padding[3];
};

//= Class Definition ===========================

/* Base class for materials. Allows for easy setting of textures and shaders. To be extended in the future for dynamic materials */
class FLOW_API Material
{
public:

	//= Public Functions ==================================

										Material();
	virtual void						BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout) const;

	void								SetTexture(const std::string& TextureName);
	void								SetPixelShader(const std::string& ShaderName);
	void								SetVertexShader(const std::string& ShaderName);

protected:

	//= Protected Variables ===============================

	TextureAsset*						m_Texture;
	ShaderAsset*						m_VertexShader;
	ShaderAsset*						m_PixelShader;
};

