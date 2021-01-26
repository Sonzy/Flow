#pragma once

//= Includes =================================================

#include "Bindable.h"
#include <DirectXMath.h>
#include "Flow/Rendering/Core/RenderQueue/Technique.h"

//= Forward Declarations =====================================

class IndexBuffer;
class BindableVertexBuffer;
class Topology;

// Struct Definitions /////////////////////////////////////////

struct SelectionPassConstantBuffer
{
	Vector4 m_guidColor;
};

//= Class Definitions ========================================

/* Interface class for anything that can be rendered*/
class FLOW_API Renderable
{
public:

	//= Public Functions ===============================

	//Renderable(Renderable&&) = delete; //TODO: Why this will stop Vector3 Unique ptr errors, but deleting lvalue copy constructor doesnt.
	virtual ~Renderable();

	virtual DirectX::XMMATRIX GetTransformXM() const = 0;

	//= Render Queue Framework ===========

	void Bind() const;
	void AddTechnique(Technique& NewTechnique);
	UINT GetIndexCount() const;

	//= Custom debug setings =

	bool									m_DebugInverseViewMatrix = false;
	bool									m_DebugNoViewMatrix = false;

protected:
	friend class Renderer;

	//= Protected Variables =============================

	IndexBuffer*							m_IndexBuffer;
	BindableVertexBuffer*					m_VertexBuffer;
	Topology*								m_Topology;
	std::vector<Technique>					m_Techniques;
};