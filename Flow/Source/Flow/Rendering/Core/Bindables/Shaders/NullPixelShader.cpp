//= Includes ===============================================

#include "pch.h"
#include "NullPixelShader.h"
#include "Flow/Rendering/Core/Bindables/BindableCodex.h"

//= Class (Null Pixel Shader) Definition ===================
NullPixelShader::NullPixelShader()
{
}

void NullPixelShader::Bind()
{
	RenderCommand::DX11GetContext()->PSSetShader(nullptr, nullptr, 0u);
}

NullPixelShader* NullPixelShader::Resolve()
{
	return BindableCodex::Resolve<NullPixelShader>();
}

std::string NullPixelShader::GenerateUID()
{
	return typeid(NullPixelShader).name();
}

std::string NullPixelShader::GetUID() const
{
	return GenerateUID();
}
