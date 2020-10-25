//= Includes ====================================

#include "Flowpch.h"
#include "InputLayout.h"
#include "Flow/ErrorHandling/ErrorMacros.h"
#include "BindableCodex.h"

//= Class (InputLayout) Definition =============

InputLayout::InputLayout(VertexLayout Layout, ID3DBlob* vertexShaderByteCode)
	: m_VertexLayout(std::move(Layout))
{
	CREATE_RESULT_HANDLE();

	const auto D3DLayout = m_VertexLayout.GetD3DLayout();
	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateInputLayout(
		D3DLayout.data(),
		(UINT)D3DLayout.size(),
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&m_InputLayout));
}

void InputLayout::Bind()
{
	RenderCommand::DX11GetContext()->IASetInputLayout(m_InputLayout.Get());
}
Bindable* InputLayout::Resolve(const VertexLayout& Layout, ID3DBlob* vertexShaderByteCode)
{
	return BindableCodex::Resolve<InputLayout>(Layout, vertexShaderByteCode);
}
std::string InputLayout::GenerateUID(const VertexLayout& Layout, ID3DBlob* vertexShaderByteCode)
{
	using namespace std::string_literals;
	return typeid(InputLayout).name() + "#"s + Layout.GetCode();
}
std::string InputLayout::GetUID() const
{
	return GenerateUID(m_VertexLayout);
}