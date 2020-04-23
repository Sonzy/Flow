#pragma once
#include "Flow\Rendering\Core\Renderable.h"
#include "Bindables/ConstantBuffers/ShaderConstantBuffers.h"

namespace Flow
{
	class VertexBuffer;
	class BindableVertexBuffer;
	class VertexLayout;
	class IndexBuffer;

	class LineBatcher : public Renderable
	{
	public:
		LineBatcher();
		~LineBatcher();

		void Initialise();

		void AddLine(Vector From, Vector To, Vector Colour);
		void Draw();
		void FlushLines();

		DirectX::XMMATRIX GetTransformXM() const override { return DirectX::XMMATRIX(); };

	private:
		VertexBuffer* VertexBuffer_;
		BindableVertexBuffer* BVertexBuffer_;
		VertexLayout* Layout_;
		IndexBuffer* IBuffer_;

		//IndexBuffer LineIndices
		std::vector<unsigned short> Indices_;

		unsigned int Lines;

		struct ViewProjBuffer
		{
			DirectX::XMMATRIX ViewProjectionMatrix_;
		} CameraMatrix;
		VertexConstantBuffer<ViewProjBuffer>* VConstantBuffer_;
	};
}