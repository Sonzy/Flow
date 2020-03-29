#pragma once
#include "Flow/Rendering/Core/RenderableBase.h"

#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Bindable.h"
#include "Flow\Rendering\Core\Bindables\IndexBuffer.h"

namespace Flow
{
	class Material;

	class FLOW_API Line : public RenderableBase
	{
	public:
		Line();

		static void Initialise();
		static void DrawLine(Vector From, Vector To, Vector Colour);

		virtual DirectX::XMMATRIX GetTransformXM() const override;

		static int Count;
	protected:

		void AddBind(std::shared_ptr<Bindable> bind);
		void BindAll();

		static VertexLayout Layout;
		static std::vector<std::shared_ptr<Bindable>> m_Binds;
		static IndexBuffer* m_IndexBuffer;


	};
}