#pragma once

//= Includes ========================================

#include "Flow/Rendering/Core/Bindable.h"

//= Class Definition ================================

/* Class used to tell DX11 to not write to the render target, only the depth etc.*/
class NullPixelShader : public Bindable
{
public:
	
	//= Public Static Functions =============================================

	static std::shared_ptr<NullPixelShader> Resolve();
	static std::string						GenerateUID();

	//= Public Functions ====================================================

											NullPixelShader();
	void									Bind() override;
	std::string								GetUID() const override;
};