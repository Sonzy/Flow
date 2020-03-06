#include "Flowpch.h"
#include "VertexShader.h"
#include <d3dcompiler.h>
#include "Flow\Rendering\Core\Bindables\BindableCodex.h"

namespace Flow
{
	VertexShader::VertexShader(const std::string& LocalPath)
		: m_AssetPath(LocalPath)
	{
		CREATE_RESULT_HANDLE();

		CATCH_ERROR_DX(D3DReadFileToBlob(std::wstring{ LocalPath.begin(),LocalPath.end() }.c_str(), &m_Blob));
		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_VertexShader));
	}

	void VertexShader::Bind()
	{
		RenderCommand::DX11GetContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	}

	ID3DBlob* VertexShader::GetByteCode() const
	{
		return m_Blob.Get();
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
		return GenerateUID(m_AssetPath);
	}
}

