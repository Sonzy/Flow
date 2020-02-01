#include "Flowpch.h"
#include "Topology.h"

namespace Flow
{
	Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
		: m_Topology(type)
	{
	}

	void Topology::Bind() noexcept
	{
		RenderCommand::DX11GetContext()->IASetPrimitiveTopology(m_Topology);
	}
}