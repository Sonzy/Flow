#pragma once
#include "Flow\Rendering\Core\Bindable.h"
#include <wrl.h>

enum class StencilMode
{
	Off,
	Write,
	Mask,
	AlwaysOnTop
};


class Stencil : public Bindable
{
public:
	Stencil(StencilMode mode);
	virtual void Bind() override;

	static std::shared_ptr<Bindable> Resolve(StencilMode mode);
	static std::string GenerateUID(StencilMode mode);
	virtual std::string GetUID() const;

	static std::string GetModeAsString(StencilMode mode);
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _Stencil;
	StencilMode _Mode;
};