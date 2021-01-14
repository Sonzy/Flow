#pragma once

//= Includes ===========================================

#include "Flow\Rendering\Core\Bindable.h"
#include <wrl.h>

//= Global Enums =======================================

enum class StencilMode
{
	Off,
	Write,
	Mask,
	AlwaysOnTop,
	NoDepth
};

//= Class Definitions ===================================

class Stencil : public Bindable
{
public:

	//= Public Static Functions ========================================
	static Bindable*					Resolve(StencilMode mode);
	static std::string									GenerateUID(StencilMode mode);
	static std::string									GetModeAsString(StencilMode mode);

public:

	//= Public Functions ===============================================

														Stencil(StencilMode mode);
	virtual void										Bind() override;
	virtual std::string									GetUID() const;


private:

	//= Private Variables ==============================================

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		m_Stencil;
	StencilMode											m_Mode;
};