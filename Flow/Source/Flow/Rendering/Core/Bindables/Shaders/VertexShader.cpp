#include "Flowpch.h"
#include "VertexShader.h"
#include <d3dcompiler.h>
#include "Flow\Rendering\Core\Bindables\BindableCodex.h"

VertexShader::VertexShader(const std::string& LocalPath)
	: _ShaderPath(LocalPath)
{
	CREATE_RESULT_HANDLE();

	CATCH_ERROR_DX(D3DReadFileToBlob(std::wstring{ LocalPath.begin(),LocalPath.end() }.c_str(), &_Blob));
	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateVertexShader(_Blob->GetBufferPointer(), _Blob->GetBufferSize(), nullptr, &_VertexShader));
}

void VertexShader::Bind()
{
	RenderCommand::DX11GetContext()->VSSetShader(_VertexShader.Get(), nullptr, 0);
}

ID3DBlob* VertexShader::GetByteCode() const
{
	return _Blob.Get();
}
std::shared_ptr<Bindable> VertexShader::Resolve(const std::string& LocalPath)
{
	return BindableCodex::Resolve<VertexShader>(LocalPath);
}
std::string VertexShader::GenerateUID(const std::string& LocalPath)
{
	using namespace std::string_literals;
	return typeid(VertexShader).name() + "#"s + LocalPath;
}
std::string VertexShader::GetUID() const
{
	return GenerateUID(_ShaderPath);
}

