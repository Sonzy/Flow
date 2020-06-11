#pragma once
#include "Flow\Core.h"
#include "Flow\GameFramework\Components\WorldComponent.h"
#include "Flow/Rendering/Core/Renderable.h"
#include <memory>

class Bindable;
class IndexBuffer;

/* Base class for any components with rendering capabilities */
class FLOW_API RenderableComponent : public WorldComponent, public Renderable
{
public:
	RenderableComponent();
	RenderableComponent(const std::string& Name);

	const IndexBuffer& GetIndexBuffer() const;

	virtual DirectX::XMMATRIX GetTransformXM() const;

protected:

	//TODO: Remove?
	virtual void RefreshBinds();
};