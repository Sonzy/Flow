#pragma once
#include "Flow/Rendering/Core/Bindable.h"

namespace Flow
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(const std::string& LocalPath);
		void Bind() override;

		//= Bindable Interface =

		static std::shared_ptr<Bindable> Resolve(const std::string& LocalPath);
		static std::string GenerateUID(const std::string& LocalPath);
		std::string GetUID() const override;

	protected:
		std::string m_AssetPath;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	};
}