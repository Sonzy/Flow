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


	class Stencil : public Bindable
	{
	public:
		Stencil(StencilMode mode);

		virtual void Bind() override;
	private:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> Stencil_;
	};
}