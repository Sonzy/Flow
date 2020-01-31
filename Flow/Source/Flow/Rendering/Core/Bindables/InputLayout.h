#pragma once
#include "Flow/Rendering/Core/Bindable.h"
#include <vector>

namespace Flow
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexShaderByteCode);
		void Bind() override;

	private:
		//Describe the input layout for the vertex shader
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	};
}