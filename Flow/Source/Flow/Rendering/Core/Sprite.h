#pragma once
#include "Flow\Rendering\Core\Renderable.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\ShaderConstantBuffers.h"

namespace Flow
{
	class ShaderAsset;
	class TextureAsset;

	class FLOW_API Sprite : public Renderable
	{
		struct SpriteMatrix
		{
			DirectX::XMMATRIX ViewpMatrix;
		};

	public:
		Sprite(TextureAsset* const Texture);

		void Update();

		//Dummy
		virtual DirectX::XMMATRIX GetTransformXM() const;

		void ControlWindow();

	protected:
		
		const DirectX::XMMATRIX World_ = DirectX::XMMatrixIdentity();

		TextureAsset* Texture_;
		const std::string VertexShaderPath_ = "Flow/Source/Flow/Rendering/Core/Shaders/SpritePS.cso";
		const std::string PixelShaderPath_ = "Flow/Source/Flow/Rendering/Core/Shaders/SpriteVS.cso";

		SpriteMatrix ConstBuffer;
		std::shared_ptr<VertexConstantBuffer<SpriteMatrix>> BoundVCB;

		bool TransposeMatrix = true;
		bool UseProjection = false;

		//Vector Scale;
	};
}