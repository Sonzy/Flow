#pragma once
#include "Flow/Rendering/Core/Bindable.h"

class Blender : public Bindable
{
public:
	Blender(bool Blending);
	virtual void Bind() override;

	static std::shared_ptr<Blender> Resolve(bool Blending);
	static std::string GenerateUID(bool Blending);
	std::string GetUID() const override;

protected:

	Microsoft::WRL::ComPtr<ID3D11BlendState> _Blender;
	bool _Blending;
};