#pragma once
#include "Flow\Rendering\Core\Bindable.h"

enum class CullMode
{
	None = 1,
	Front = 2,
	Back = 3
};

class Rasterizer : public Bindable
{
public:
	Rasterizer(CullMode CullMode);
	void Bind() override;

	static std::shared_ptr<Rasterizer> Resolve(CullMode CullMode);
	static std::string GenerateUID(CullMode CullMode);

protected:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> _Rasterizer;
	CullMode _CullMode;
};