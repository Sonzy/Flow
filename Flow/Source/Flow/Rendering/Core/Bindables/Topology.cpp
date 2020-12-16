//= Includes =========================================

#include "Flowpch.h"
#include "Topology.h"
#include "BindableCodex.h"

//= Class (Topology) Definition =====================

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
	: m_Topology(type)
{
	m_CheckBound = true;
	m_Bound = false;
}

void Topology::Bind() noexcept
{
	RenderCommand::DX11GetContext()->IASetPrimitiveTopology(m_Topology);
}

Topology* Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY type)
{
	return BindableCodex::Resolve<Topology>(type);
}

std::string Topology::GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type)
{
	using namespace std::string_literals;
	return typeid(Topology).name() + "#"s + std::to_string(type);
}

std::string Topology::GetUID() const
{
	return GenerateUID(m_Topology);
}