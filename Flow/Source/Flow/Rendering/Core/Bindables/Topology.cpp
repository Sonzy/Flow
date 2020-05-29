#include "Flowpch.h"
#include "Topology.h"
#include "BindableCodex.h"

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
	: _Topology(type)
{
	_CheckBound = true;
	_Bound = false;
}

void Topology::Bind() noexcept
{
	RenderCommand::DX11GetContext()->IASetPrimitiveTopology(_Topology);
}

std::shared_ptr<Bindable> Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY type)
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
	return GenerateUID(_Topology);
}