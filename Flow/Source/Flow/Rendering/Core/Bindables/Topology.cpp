// Pch ////////////////////////////////////////////////////////////////////////

#include "pch.h"

// Includes ////////////////////////////////////////////////////////////////////

#include "Topology.h"
#include "Rendering/Renderer.h"
#include "Rendering/Core/Bindables/Codex.h"

// Function Definitions //////////////////////////////////////////////////////

Bindables::Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
	: m_topology(type)
{
}

void Bindables::Topology::Bind()
{
	Renderer::GetContext()->IASetPrimitiveTopology(m_topology);
}

Bindables::Topology* Bindables::Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY type)
{
	return Bindables::Codex::Resolve<Topology>(type);
}

HashString Bindables::Topology::GenerateID(D3D11_PRIMITIVE_TOPOLOGY type)
{
	char buffer[64];
	snprintf(buffer, 64, "Topology-%d", static_cast<int>(type));
	return buffer;
}

HashString Bindables::Topology::GetID()
{
	if (m_id.IsNull())
	{
		m_id = GenerateID(m_topology);
	}
	return m_id;
}