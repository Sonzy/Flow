#include "Topology.h"

namespace Flow
{
	Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
		: m_Topology(type)
	{
	}

	void Topology::Bind() noexcept
	{
		//TODO: 	GetContext(gfx)->IASetPrimitiveTopology(type);
	}
}