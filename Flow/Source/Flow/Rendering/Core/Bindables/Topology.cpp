#include "Flowpch.h"
#include "Topology.h"

namespace Flow
{
	Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
		: m_Topology(type)
	{
		bCheckBound = true;
		bBound = false;
	}

	void Topology::Bind() noexcept
	{
		RenderCommand::DX11GetContext()->IASetPrimitiveTopology(m_Topology);
	}
}