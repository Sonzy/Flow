#pragma once

//= Includes =================================================

#include <DirectXMath.h>
#include "Rendering\Core\Bindables\ConstantBuffers\ShaderConstantBuffers.h"
#include "Rendering\Core\Mesh\StaticMesh.h"

//= Class Definition =========================================

class FLOW_API PointLight
{
public:
	
	//= Public Functions =================================================

											PointLight(float LightRadius = 0.5f);
	void									Reset();
	void									BindLight(DirectX::FXMMATRIX ViewMatrix);
	void									RenderControlWindow();
	StaticMesh*								GetMesh()	{ return nullptr; }

private:

	//= Private Structs ==================================================

	struct LightConstantBuffer
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};

private:

	//= Private Variables ================================================

	LightConstantBuffer							m_ConstantBuffer;
	PixelConstantBuffer<LightConstantBuffer>	m_PixelConstantBuffer;
};