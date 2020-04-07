#pragma once
#include "Flow/Rendering/Core/Bindable.h"

namespace Flow
{
	class Sampler : public Bindable
	{
	public:
		Sampler();
		void Bind() override;


		//= Bindable Interface =

		static std::shared_ptr<Bindable> Resolve();
		static std::string GenerateUID();
		std::string GetUID() const override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> Sampler_;
	};
}
