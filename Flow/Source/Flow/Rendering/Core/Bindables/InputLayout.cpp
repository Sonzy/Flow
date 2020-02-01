#include "Flowpch.h"
#include "InputLayout.h"
#include "Flow/ErrorHandling/ErrorMacros.h"

namespace Flow
{
	InputLayout::InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexShaderByteCode)
	{
		CREATE_RESULT_HANDLE();

		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateInputLayout(
			layout.data(),
			(UINT)layout.size(),
			vertexShaderByteCode->GetBufferPointer(),
			vertexShaderByteCode->GetBufferSize(),
			&m_InputLayout));
	}

	void InputLayout::Bind()
	{
		RenderCommand::DX11GetContext()->IASetInputLayout(m_InputLayout.Get());
	}
}