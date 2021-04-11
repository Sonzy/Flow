#pragma once

//= Includes =================================================

#include <DirectXMath.h>
#include "Bindables/Bindable.h"
#include "Rendering/Core/RenderQueue/Technique.h"
#include "Maths\Vector4.h"

//= Forward Declarations =====================================

namespace Bindables
{
	class IndexBuffer;
	class VertexBuffer;
	class Topology;
}

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

	Bindables::IndexBuffer*					m_indexBuffer;
	Bindables::VertexBuffer*				m_vertexBuffer;
	Bindables::Topology*					m_topology;
	std::vector<Technique>					m_techniques;
};