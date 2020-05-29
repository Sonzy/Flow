#pragma once
#include "Flow\Rendering\Core\Bindable.h"

class Rasterizer : public Bindable
{
public:
	Rasterizer(bool DoubleSided);
	void Bind() override;

	static std::shared_ptr<Rasterizer> Resolve(bool DoubleSided);
	static std::string GenerateUID(bool DoubleSided);

protected:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> _Rasterizer;
	bool _DoubleSided;
};