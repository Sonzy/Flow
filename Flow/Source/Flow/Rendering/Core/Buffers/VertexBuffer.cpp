#include "Flowpch.h"
#include "VertexBuffer.h"
#include "Flow/Rendering/Renderer.h"
#include "Flow/Window/Window.h"

namespace Flow
{
	VertexBuffer* VertexBuffer::Create(float* Vertices, size_t BufferSize)
	{
		//TODO: Do buffers lol
		return nullptr;
	}

	void VertexBuffer::Bind()
	{
		const UINT offset = 0;

		switch (Renderer::GetRenderAPI())
		{
		case RenderAPI::DirectX11:

			break;

		default:
			break;
		}
	}
}


