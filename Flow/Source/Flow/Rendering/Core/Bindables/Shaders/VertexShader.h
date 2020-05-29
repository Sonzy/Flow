#pragma once
#include "Flow/Rendering/Core/Bindable.h"

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
	std::string _ShaderPath;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> _VertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> _Blob;
};