#pragma once
#include "Flow/Rendering/Core/RenderableBase.h"

#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Bindable.h"
#include "Flow\Rendering\Core\Bindables\IndexBuffer.h"

class Material;

class FLOW_API Line : public RenderableBase
{
public:
	Line();

	static void Initialise();
	static void DrawLine(Vector3 From, Vector3 To, Vector3 Colour);

	virtual DirectX::XMMATRIX GetTransformXM() const override;

	static int Count;
protected:

	void AddBind(std::shared_ptr<Bindable> bind);
	void BindAll();

	static VertexLayout VertexLayout_;
	static std::vector<std::shared_ptr<Bindable>> Binds_;
	static IndexBuffer* IndexBuffer_;


};