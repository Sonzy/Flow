#include "pch.h"
#include "Renderable.h"
#include "Flow/Rendering/Core/Bindables/IndexBuffer.h"
#include "Flow/Rendering/Core/Bindables/VertexBuffer.h"
#include "Flow/Rendering/Core/Bindables/Topology.h"


void Renderable::Bind() const
{
	m_indexBuffer->Bind();
	m_vertexBuffer->Bind();
	m_topology->Bind();
}

void Renderable::AddTechnique(Technique& NewTechnique)
{
	NewTechnique.InitialiseParentReferences(*this);
	m_techniques.push_back(std::move(NewTechnique));
}

UINT Renderable::GetIndexCount() const
{
	return m_indexBuffer->GetCount();
}

Renderable::~Renderable()
{
	m_techniques.clear();
}