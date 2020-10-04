#pragma once

//= Includes =================================================

#include "Bindable.h"
#include <DirectXMath.h>
#include "Flow/Rendering/Core/RenderQueue/Technique.h"

//= Forward Declarations =====================================

class IndexBuffer;
class BindableVertexBuffer;
class Topology;

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

protected:
	friend class Renderer;

	//= Protected Variables =============================

	std::shared_ptr<IndexBuffer>			m_IndexBuffer;
	std::shared_ptr<BindableVertexBuffer>	m_VertexBuffer;
	std::shared_ptr<Topology>				m_Topology;
	std::vector<Technique>					m_Techniques;
};