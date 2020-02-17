#include "Flowpch.h"
#include "StaticMeshComponent.h"

#include "Flow\Rendering\Renderer.h"

namespace Flow
{
	StaticMeshComponent::StaticMeshComponent()
	{
	}

	void StaticMeshComponent::InitialiseComponent(const std::string& MeshName)
	{
		m_StaticMesh = std::make_shared<StaticMesh>(MeshName);
	}

	void StaticMeshComponent::Tick(float DeltaTime)
	{
		GameObject::Tick(DeltaTime);

		Renderer::Submit(m_StaticMesh.get());
	}
}