#pragma once
#include "Flow/Rendering/Core/Bindable.h"

/* Class used to tell DX11 to not write to the render target, only the depth etc.*/
class NullPixelShader : public Bindable
{
public:

	NullPixelShader();
	void Bind() override;

	static std::shared_ptr<NullPixelShader> Resolve();
	static std::string GenerateUID();
	std::string GetUID() const override;
};