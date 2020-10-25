#pragma once

//= Includes =======================================

#include "Flow/Rendering/Core/Bindable.h"

//= Class Definition ===============================

class Sampler : public Bindable
{
public:

	//= Public Static Functions ======================================

	static Bindable*		Resolve();
	static std::string						GenerateUID();
public:

	//= Public Functions =============================================

											Sampler();
	void									Bind() override;
	std::string								GetUID() const override;
private:

	//= Private Variables ============================================

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		m_Sampler;
};