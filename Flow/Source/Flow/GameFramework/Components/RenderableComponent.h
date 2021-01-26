#pragma once

//= Includes =====================================================

#include "Flow\Core.h"
#include "Flow\GameFramework\Components\WorldComponent.h"
#include "Flow/Rendering/Core/Renderable.h"
#include <memory>

//= Forward Declarations =========================================

class Bindable;
class IndexBuffer;

//TODO: Move

struct SelectionPassConstantBuffer
{
	Vector4 selectionColor;
};

//= Class Definitions ============================================

/* Base class for any components with rendering capabilities */
class FLOW_API RenderableComponent : public WorldComponent, public Renderable
{
public:

	REGISTER_CLASS(RenderableComponent)

	//= Public Functions ==========================

								RenderableComponent();
								RenderableComponent(const std::string& Name);

	const IndexBuffer&			GetIndexBuffer() const;

	virtual DirectX::XMMATRIX	GetTransformXM() const;

	virtual void				OnRegistered() override;

protected:

	//= Protected Functions =======================

	virtual void				RefreshBinds();

	SelectionPassConstantBuffer m_SelectionConstantBuffer;
};