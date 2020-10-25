//= Includes =======================================

#include "Flowpch.h"
#include "Rasterizer.h"
#include "BindableCodex.h"

//= Class (Rasterizer) Definition ==================

Rasterizer::Rasterizer(CullMode CullMode)
	: m_CullMode(CullMode)
{
	CREATE_RESULT_HANDLE();

	CD3D11_RASTERIZER_DESC Description = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	Description.CullMode = static_cast<D3D11_CULL_MODE>(m_CullMode); 
	//TODO: Need to flip the normals in the shaders if it is a back face

	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateRasterizerState(&Description, &m_Rasterizer));
}

void Rasterizer::Bind()
{
	RenderCommand::DX11GetContext()->RSSetState(m_Rasterizer.Get());
}

Rasterizer* Rasterizer::Resolve(CullMode CullMode)
{
	return BindableCodex::Resolve<Rasterizer>(CullMode);
}

std::string Rasterizer::GenerateUID(CullMode CullMode)
{
	using namespace std::string_literals;
	std::string Type;
	switch (CullMode)
	{
	case CullMode::None:
		Type = "None";
		break;
	case CullMode::Back:
		Type = "Back";
		break;
	case CullMode::Front:
		Type = "Front";
		break;
	default:
		break;
	}

	return typeid(Rasterizer).name() + "#"s + Type;
}