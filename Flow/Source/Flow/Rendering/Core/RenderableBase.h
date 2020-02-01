#pragma once

#include "Renderable.h"
#include "Bindables/BindableVertexBuffer.h"
#include "Bindables/IndexBuffer.h"
#include "Bindables/Sampler.h"
#include "Bindables/InputLayout.h"
#include "Bindables/Shaders/VertexShader.h"
#include "Bindables/Shaders/PixelShader.h"
#include "Bindables/ConstantBuffers/ConstantBuffer.h"
#include "Bindables/ConstantBuffers/ShaderConstantBuffers.h"
#include "Bindables/ConstantBuffers/TransformConstantBuffer.h"

namespace Flow
{
	/* Base class for anything renderable, includes all the includes for the DX11 pipeline */
	class FLOW_API RenderableBase : public Renderable
	{

	};
}