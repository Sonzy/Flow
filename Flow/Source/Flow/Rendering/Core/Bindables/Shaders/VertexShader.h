#pragma once
#include "Flow/Rendering/Core/Bindable.h"

namespace Flow
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(const std::wstring& LocalPath);

		void Bind() override;

		ID3DBlob* GetByteCode() const;

	protected:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_Blob;
	};
}