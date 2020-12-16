//= Includes ===================================================

#include "Flowpch.h"
#include <d3dcompiler.h>
#include "PixelShader.h"
#include "Rendering\Core\Bindables\BindableCodex.h"

//= Class (Pixel Shader) Definition =============================

PixelShader::PixelShader(const std::string& LocalPath)
	: m_ShaderPath(LocalPath)
{
	CREATE_RESULT_HANDLE();

	Microsoft::WRL::ComPtr<ID3DBlob> Blob;
	CATCH_ERROR_DX(D3DReadFileToBlob(std::wstring{ LocalPath.begin(),LocalPath.end() }.c_str(), &Blob));
	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreatePixelShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, &m_PixelShader));
}

void PixelShader::Bind()
{
	RenderCommand::DX11GetContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0);
}

PixelShader* PixelShader::Resolve(const std::string& LocalPath)
{
	return BindableCodex::Resolve<PixelShader>(LocalPath);
}

std::string PixelShader::GenerateUID(const std::string& LocalPath)
{
	using namespace std::string_literals;
	return typeid(PixelShader).name() + "#"s + LocalPath;
}

std::string PixelShader::GetUID() const
{
	return GenerateUID(m_ShaderPath);
}