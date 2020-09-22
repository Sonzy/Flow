#pragma once
#include "Flow\Rendering\Core\Renderable.h"
#include "Flow/Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"

class VertexBuffer;
class BindableVertexBuffer;
class VertexLayout;


class LineBatcher : public Renderable
{
public:

	LineBatcher();
	~LineBatcher();

	void Initialise();
	void AddLine(Vector3 From, Vector3 To, Vector3 Colour = Vector3(1.0f, 1.0f, 1.0f));
	void DrawLines();
	void FlushLines();

	unsigned int GetNumberOfLines() const { return Lines; }

	//Dummy override
	DirectX::XMMATRIX GetTransformXM() const override { return DirectX::XMMATRIX(); };


private:
	void BindAll();
	void AddBind(std::shared_ptr<Bindable> NewBind);
	std::vector<std::shared_ptr<Bindable>> _Binds;

	VertexBuffer* _VertexBuffer;
	BindableVertexBuffer* _BindableVertexBuffer;
	VertexLayout* _VertexLayout;

	struct ViewProjectionBuffer
	{
		DirectX::XMMATRIX _ViewProjectionMatrix;
	} _CameraMatrix;
	VertexConstantBuffer<ViewProjectionBuffer>* _VertexCB;

	unsigned int Lines;
};