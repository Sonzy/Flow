#pragma once

#include <DirectXMath.h>
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\ShaderConstantBuffers.h"
#include "Flow\Rendering\Core\Mesh\StaticMesh.h"

namespace Flow
{
	class FLOW_API PointLight
	{
	public:
		PointLight(float LightRadius = 0.5f);

		void Reset();

		void Bind(DirectX::FXMMATRIX ViewMatrix);

		void RenderControlWindow();

		StaticMesh* GetMesh() { return &m_Mesh; }
		
	private:

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
		StaticMesh m_Mesh;

		LightConstantBuffer m_ConstantBuffer;
		PixelConstantBuffer<LightConstantBuffer> m_PixelConstantBuffer;
	};
}