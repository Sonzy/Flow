#pragma once
#include "Flow/Rendering/Core/Bindable.h"
#include <vector>
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"

namespace Flow
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(VertexLayout Layout, ID3DBlob* vertexShaderByteCode);
		~InputLayout();
		void Bind() override;

		//= Bindable Interface =

		static std::shared_ptr<Bindable> Resolve(const VertexLayout& Layout , ID3DBlob* vertexShaderByteCode);
		static std::string GenerateUID(const VertexLayout& Layout , ID3DBlob* vertexShaderByteCode = nullptr);
		std::string GetUID() const override;

	private:
		//Describe the input layout for the vertex shader
		Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout_;
		VertexLayout VertexLayout_;
	};
}