#include "Flowpch.h"
#include "Renderable.h"
#include "Flow/Rendering/Core/Bindables/IndexBuffer.h"
#include "Flow/Rendering/Core/Bindables/BindableVertexBuffer.h"
#include "Flow/Rendering/Core/Bindables/Topology.h"


void Renderable::Bind() const
{
	_IndexBuffer->Bind();
	_VertexBuffer->Bind();
	_Topology->Bind();
}

void Renderable::AddTechnique(Technique& NewTechnique)
{
	NewTechnique.InitialiseParentReferences(*this);
	_Techniques.push_back(std::move(NewTechnique));
}

UINT Renderable::GetIndexCount() const
{
	return _IndexBuffer->GetCount();
}

Renderable::~Renderable()
{
	_Techniques.clear();
}