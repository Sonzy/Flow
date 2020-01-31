#pragma once
#include "Flow/Rendering/Core/Bindable.h"

namespace Flow
{
	class Topology : public Bindable
	{
	public:
		Topology(D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind() noexcept override;

	protected:
		D3D11_PRIMITIVE_TOPOLOGY m_Topology;
	};
}