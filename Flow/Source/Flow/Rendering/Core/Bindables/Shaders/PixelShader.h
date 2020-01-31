#pragma once
#include "Flow/Rendering/Core/Bindable.h"

namespace Flow
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(const std::wstring& LocalPath);
		void Bind() override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	};
}