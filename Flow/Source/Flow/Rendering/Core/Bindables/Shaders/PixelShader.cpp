#include "Flowpch.h"
#include "PixelShader.h"
#include <d3dcompiler.h>
#include "Flow\Rendering\Core\Bindables\BindableCodex.h"

namespace Flow
{
	PixelShader::PixelShader(const std::string& LocalPath)
		: m_AssetPath(LocalPath)
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
	std::shared_ptr<Bindable> PixelShader::Resolve(const std::string& LocalPath)
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
		return GenerateUID(m_AssetPath);
	}
}