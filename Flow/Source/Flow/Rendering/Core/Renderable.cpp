#include "pch.h"
#include "Renderable.h"
#include "Flow/Rendering/Core/Bindables/IndexBuffer.h"
#include "Flow/Rendering/Core/Bindables/BindableVertexBuffer.h"
#include "Flow/Rendering/Core/Bindables/Topology.h"


void Renderable::Bind() const
{
	m_IndexBuffer->Bind();
	m_VertexBuffer->Bind();
	m_Topology->Bind();
}

void Renderable::AddTechnique(Technique& NewTechnique)
{
	NewTechnique.InitialiseParentReferences(*this);
	m_Techniques.push_back(std::move(NewTechnique));
}

UINT Renderable::GetIndexCount() const
{
	return m_IndexBuffer->GetCount();
}

Renderable::~Renderable()
{
	m_Techniques.clear();
}