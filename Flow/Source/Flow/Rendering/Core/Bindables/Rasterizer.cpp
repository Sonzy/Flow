#include "Flowpch.h"
#include "Rasterizer.h"
#include "BindableCodex.h"

namespace Flow
{
	Rasterizer::Rasterizer(bool DoubleSided)
		: DoubleSided_(DoubleSided)
	{
		CREATE_RESULT_HANDLE();

		CD3D11_RASTERIZER_DESC Description = CD3D11_RASTERIZER_DESC( CD3D11_DEFAULT{} );
		Description.CullMode = DoubleSided ? D3D11_CULL_NONE : D3D11_CULL_BACK; //TODO: Cba for now but should really just cull front for the skybox

		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateRasterizerState(&Description, &Rasterizer_));
	}

	void Rasterizer::Bind()
	{
		RenderCommand::DX11GetContext()->RSSetState(Rasterizer_.Get());
	}

	std::shared_ptr<Rasterizer> Rasterizer::Resolve(bool DoubleSided)
	{
		return BindableCodex::Resolve<Rasterizer>(DoubleSided);
	}
	std::string Rasterizer::GenerateUID(bool DoubleSided)
	{
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + (DoubleSided ? "Double" : "Single");
	}
}