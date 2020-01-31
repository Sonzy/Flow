#pragma once
#include "Flow/Rendering/Core/Bindable.h"

namespace Flow
{
	class VertexBuffer : public Bindable
	{
	public:
		virtual ~VertexBuffer() {}

		/* Creates a vertex buffer, pass in as a float array to abstract between APIs*/
		static VertexBuffer* Create(float* Vertices, size_t BufferSize);

		virtual void Bind() override;
	};
}