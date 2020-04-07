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

		void BindLight(DirectX::FXMMATRIX ViewMatrix);

		void RenderControlWindow();

		StaticMesh* GetMesh() { return &Mesh_; }
		
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
		StaticMesh Mesh_;
		bool DrawMesh_ = true;

		LightConstantBuffer ConstantBuffer_;
		PixelConstantBuffer<LightConstantBuffer> PixelConstantBuffer_;
	};
}