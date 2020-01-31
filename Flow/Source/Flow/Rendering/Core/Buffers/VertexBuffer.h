#pragma once

namespace Flow
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		static VertexBuffer* Create();
	};
}