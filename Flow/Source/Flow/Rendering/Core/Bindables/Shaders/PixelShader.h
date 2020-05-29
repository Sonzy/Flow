#pragma once
#include "Flow/Rendering/Core/Bindable.h"

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
	std::string _ShaderPath;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> _PixelShader;
};