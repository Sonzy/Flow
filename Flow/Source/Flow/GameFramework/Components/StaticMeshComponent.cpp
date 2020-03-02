#include "Flowpch.h"
#include "StaticMeshComponent.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"

#include "Flow\Rendering\Renderer.h"

namespace Flow
{
	StaticMeshComponent::StaticMeshComponent()
		: m_StaticMesh(nullptr)
	{
	}

	void StaticMeshComponent::InitialiseComponent(const std::string& MeshName)
	{
		m_StaticMesh = new StaticMesh(MeshName);
	}

	void StaticMeshComponent::Tick(float DeltaTime)
	{
		GameObject::Tick(DeltaTime);
	}

	void StaticMeshComponent::SetStaticMesh(const std::string& MeshName)
	{
		//TODO: Change mesh at runtime without creating new object
		if (m_StaticMesh)
			delete m_StaticMesh;

		m_StaticMesh = new StaticMesh(MeshName);
	}

	void StaticMeshComponent::Render()
	{
		Flow::Renderer::Submit(m_StaticMesh);
	}
}