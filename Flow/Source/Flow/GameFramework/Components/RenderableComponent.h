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

	REGISTER_CLASS(RenderableComponent)

	//= Public Functions ==========================

								RenderableComponent();
								RenderableComponent(const std::string& Name);

	const IndexBuffer&			GetIndexBuffer() const;

	virtual DirectX::XMMATRIX	GetTransformXM() const;

	void						SetMatrix(DirectX::XMFLOAT4X4 fMatrix);
	DirectX::XMMATRIX			GetMatrix() const { return m_matrix; }


	//TODO: Shouldnt need this
	bool						UsingMatrixRotation() const { return m_useMatrix; }

protected:

	//= Protected Functions =======================

	virtual void				RefreshBinds();


	//TODO: Udpate rotation mode
	bool						m_useMatrix;
	DirectX::XMMATRIX			m_matrix;
};