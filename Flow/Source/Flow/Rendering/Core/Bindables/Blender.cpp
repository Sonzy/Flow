#include "Flowpch.h"
#include "Blender.h"
#include "Flow/Rendering/Core/Bindables/BindableCodex.h"

Blender::Blender(bool Blending)
	: _Blending(Blending)
{
	D3D11_BLEND_DESC BlendDescription = {};
	auto& RenderTarget = BlendDescription.RenderTarget[0];

	if (_Blending)
	{
		RenderTarget.BlendEnable = TRUE;
		RenderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		RenderTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		RenderTarget.BlendOp = D3D11_BLEND_OP_ADD;
		RenderTarget.SrcBlendAlpha = D3D11_BLEND_ZERO;
		RenderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
		RenderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		RenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	else
	{
		RenderTarget.BlendEnable = FALSE;
		RenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	CREATE_RESULT_HANDLE();
	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateBlendState(&BlendDescription, &_Blender));
}

void Blender::Bind()
{
	RenderCommand::DX11GetContext()->OMSetBlendState(_Blender.Get(), nullptr, 0xFFFFFFFFu);
}

std::shared_ptr<Blender> Blender::Resolve(bool Blending)
{
	return BindableCodex::Resolve<Blender>(Blending);
}

std::string Blender::GenerateUID(bool Blending)
{
	using namespace std::string_literals;
	return typeid(Blender).name() + (Blending ? "True"s : "False"s);
}

std::string Blender::GetUID() const
{
	return GenerateUID(_Blending);
}