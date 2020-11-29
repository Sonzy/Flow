#pragma once

//= Includes =====================================================

#include "Flow\Core.h"
#include "Flow\GameFramework\Components\WorldComponent.h"
#include "Flow/Rendering/Core/Renderable.h"
#include <memory>

//= Forward Declarations =========================================

class Bindable;
class IndexBuffer;

//= Class Definitions ============================================

/* Base class for any components with rendering capabilities */
class FLOW_API RenderableComponent : public WorldComponent, public Renderable
{
public:

	//= Public Functions ==========================

								RenderableComponent();
								RenderableComponent(const std::string& Name);

	const IndexBuffer&			GetIndexBuffer() const;

	virtual DirectX::XMMATRIX	GetTransformXM() const;

protected:

	//= Protected Functions =======================

	virtual void				RefreshBinds();
};