#pragma once
#include "Flow\Rendering\Core\Bindable.h"
#include <wrl.h>

namespace Flow
{
	enum class StencilMode
	{
		Off,
		Write,
		Mask
	};

	enum class DepthMode
	{
		Off,
		On
	};


	class Stencil : public Bindable
	{
	public:
		Stencil(StencilMode mode, DepthMode Depth = DepthMode::On);

		virtual void Bind() override;
	private:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> Stencil_;
	};
}