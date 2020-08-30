#pragma once
#include "Bindable.h"
#include <DirectXMath.h>
#include "Flow/Rendering/Core/RenderQueue/Technique.h"

class IndexBuffer;
class BindableVertexBuffer;
class Topology;

/* Interface class for anything that can be rendered*/
class FLOW_API Renderable
{
public:
	//Renderable(Renderable&&) = delete; //TODO: Why this will stop Vector Unique ptr errors, but deleting lvalue copy constructor doesnt.
	virtual ~Renderable();

	virtual DirectX::XMMATRIX GetTransformXM() const = 0;

	//= Render Queue Framework ===========

	void Bind() const;
	void AddTechnique(Technique& NewTechnique);
	UINT GetIndexCount() const;

protected:
	friend class Renderer;

	std::shared_ptr<IndexBuffer> _IndexBuffer;
	std::shared_ptr<BindableVertexBuffer> _VertexBuffer;
	std::shared_ptr<Topology> _Topology;
	std::vector<Technique> _Techniques;
};