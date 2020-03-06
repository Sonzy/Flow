#include "Flowpch.h"
#include "StaticMeshComponent.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"

#include "Flow\Rendering\Renderer.h"

namespace Flow
{
	StaticMeshComponent::StaticMeshComponent()
		: WorldComponent("Unnamed StaticMesh Component"), m_StaticMesh(nullptr)
	{
	}

	StaticMeshComponent::StaticMeshComponent(const std::string& Name)
		: WorldComponent(Name), m_StaticMesh(nullptr)
	{
	}

	StaticMeshComponent::~StaticMeshComponent()
	{
		delete m_StaticMesh;
	}

	void StaticMeshComponent::InitialiseComponent(const std::string& MeshName, Material* OverrideMaterial)
	{
		bOverrideMaterial = (bool)OverrideMaterial;

		if (OverrideMaterial)
			SetMaterial(OverrideMaterial);

		SetStaticMesh(MeshName);
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
		m_StaticMesh->InitialiseStaticMesh(MeshName, bOverrideMaterial ? m_Material : nullptr);
	}

	void StaticMeshComponent::SetMaterial(Material* NewMaterial)
	{
		m_Material = NewMaterial;
	}

	void StaticMeshComponent::Render()
	{
		Flow::Renderer::Submit(m_StaticMesh);
	}
}