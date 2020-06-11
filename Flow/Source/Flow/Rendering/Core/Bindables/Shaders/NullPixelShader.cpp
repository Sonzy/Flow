#include "Flowpch.h"
#include "NullPixelShader.h"
#include "Flow/Rendering/Core/Bindables/BindableCodex.h"

NullPixelShader::NullPixelShader()
{
}

void NullPixelShader::Bind()
{
	RenderCommand::DX11GetContext()->PSSetShader(nullptr, nullptr, 0u);
}

std::shared_ptr<NullPixelShader> NullPixelShader::Resolve()
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
