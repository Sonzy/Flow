#pragma once

//= Includes =======================================

#include <vector>
#include "Rendering/Core/Bindable.h"
#include "Rendering/Core/Vertex/VertexLayout.h"

//= Class Definition ================================

class InputLayout : public Bindable
{
public:

	//= Public Static Functions ========================================

	static std::shared_ptr<Bindable>		Resolve(const VertexLayout& Layout, ID3DBlob* vertexShaderByteCode);
	static std::string						GenerateUID(const VertexLayout& Layout, ID3DBlob* vertexShaderByteCode = nullptr);

	//= Public Functions ===============================================

											InputLayout(VertexLayout Layout, ID3DBlob* vertexShaderByteCode);
	void									Bind() override;
	std::string								GetUID() const override;

private:
	
	//= Private Variables ===============================================

	//Describe the input layout for the vertex shader
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_InputLayout;
	VertexLayout									m_VertexLayout;
};