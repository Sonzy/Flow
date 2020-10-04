#pragma once

//= Includes =======================================

#include "Rendering/Core/Bindable.h"
#include "Rendering/Core/Bindables/IndexBuffer.h"
#include "Rendering/Core/RenderableBase.h"
#include "Rendering/Core/Vertex/VertexLayout.h"

//= Forward Declarations ===========================

class Material;

//= Class Definition ===============================

class FLOW_API Line : public RenderableBase
{
public:

	//= Public Static Functions ==================================

	static void					Initialise();
	static void					DrawLine(Vector3 From, Vector3 To, Vector3 Colour);

public:

	//= Public Functions =========================================

								Line();
	virtual DirectX::XMMATRIX	GetTransformXM() const override;

public:

	//= Public Variables =========================================

	static int					m_Count;

private:

	//= Private Functions ========================================
	void						AddBind(std::shared_ptr<Bindable> bind);
	void						BindAll();

private:

	//= Private Static Variables =================================

	static VertexLayout								m_VertexLayout;
	static std::vector<std::shared_ptr<Bindable>>	m_Binds;
	static IndexBuffer*								m_IndexBuffer;


};