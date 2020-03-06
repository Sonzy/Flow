#pragma once
#include "Flow/Rendering/Core/Bindable.h"

namespace Flow
{
	class Topology : public Bindable
	{
	public:
		Topology(D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind() noexcept override;

		//= Bindable Interface =

		static std::shared_ptr<Bindable> Resolve(D3D11_PRIMITIVE_TOPOLOGY type);
		static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);
		std::string GetUID() const override;
	protected:
		D3D11_PRIMITIVE_TOPOLOGY m_Topology;
	};
}