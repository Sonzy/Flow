#pragma once
#include "Flow/Rendering/Core/Bindable.h"

namespace Flow
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(const std::string& LocalPath);

		void Bind() override;

		ID3DBlob* GetByteCode() const;

		//= Bindable Interface =

		static std::shared_ptr<Bindable> Resolve(const std::string& LocalPath);
		static std::string GenerateUID(const std::string& LocalPath);
		std::string GetUID() const override;

	protected:
		std::string ShaderPath_;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader_;
		Microsoft::WRL::ComPtr<ID3DBlob> Blob_;
	};
}