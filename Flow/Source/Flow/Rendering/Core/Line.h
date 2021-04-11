#pragma once

//= Includes =======================================

#include "Rendering/Core/Bindables/Bindable.h"
#include "Rendering/Core/Bindables/IndexBuffer.h"
#include "Rendering/Core/Renderable.h"
#include "Rendering/Core/Vertex/VertexLayout.h"

//= Forward Declarations ===========================

class Material;

//= Class Definition ===============================

class FLOW_API Line : public Renderable
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
	void						AddBind(Bindables::Bindable* bind);
	void						BindAll();

private:

	//= Private Static Variables =================================

	static VertexLayout								m_VertexLayout;
	static std::vector<Bindables::Bindable*>		m_Binds;
	static Bindables::IndexBuffer*					m_IndexBuffer;


};