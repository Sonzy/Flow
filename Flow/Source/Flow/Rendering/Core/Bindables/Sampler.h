#pragma once
#include "Flow/Rendering/Core/Bindable.h"

namespace Flow
{
	class Sampler : public Bindable
	{
	public:
		Sampler();
		void Bind() override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_Sampler;
	};
}
