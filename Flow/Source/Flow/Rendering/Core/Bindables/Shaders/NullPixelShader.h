#pragma once

//= Includes ========================================

#include "Flow/Rendering/Core/Bindables/Bindable.h"

//= Class Definition ================================

/* Class used to tell DX11 to not write to the render target, only the depth etc.*/
class NullPixelShader : public Bindables::Bindable
{
public:
	
	//= Public Static Functions =============================================

	static NullPixelShader*					Resolve();
	static HashString						GenerateID();

	//= Public Functions ====================================================

											NullPixelShader();
	void									Bind() override;
	HashString								GetID() override;
};