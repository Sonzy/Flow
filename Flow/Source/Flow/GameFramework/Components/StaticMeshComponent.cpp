#include "Flowpch.h"
#include "StaticMeshComponent.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"

namespace Flow
{
	StaticMeshComponent::StaticMeshComponent()
		: RenderableComponent("Unnamed StaticMesh Component"), m_StaticMesh(nullptr), m_Material(nullptr)
	{
	}

	StaticMeshComponent::StaticMeshComponent(const std::string& Name, MeshAsset* Mesh, Material* Material)
		: RenderableComponent(Name), m_StaticMesh(nullptr), m_Material(nullptr)
	{
		if (Mesh && Material)
			SetMeshAndMaterial(Mesh, Material);
	}

	StaticMeshComponent::~StaticMeshComponent()
	{
		delete m_StaticMesh;
	}

	void StaticMeshComponent::InitialiseComponent(MeshAsset* Mesh, Material* Material)
	{
		SetMeshAndMaterial(Mesh, Material);
	}

	void StaticMeshComponent::Tick(float DeltaTime)
	{
		GameObject::Tick(DeltaTime);
	}

	void StaticMeshComponent::SetMeshAndMaterial(MeshAsset* Mesh, Material* Material)
	{
		CHECK_RETURN(!Mesh, "StaticMeshComponent::SetMeshAndMaterial: Mesh was nullptr");
		CHECK_RETURN(!Material, "StaticMeshComponent::SetMeshAndMaterial: Material was nullptr");

		m_StaticMesh = Mesh;
		m_Material = Material;

		RefreshBinds();
	}

	void StaticMeshComponent::SetStaticMesh(const std::string& MeshName)
	{
		//TODO: Change mesh at runtime without creating new object
		MeshAsset* NewAsset = AssetSystem::GetAsset<MeshAsset>(MeshName);
		CHECK_RETURN(!NewAsset, "StaticMeshComponent::SetStaticMesh: Failed to get new static mesh.");

		m_StaticMesh = NewAsset;

		RefreshBinds(); //TODO: Dont call this twice? dunno
	}

	void StaticMeshComponent::SetMaterial(Material* NewMaterial)
	{
		m_Material = NewMaterial;

		RefreshBinds();
	}

	void StaticMeshComponent::Render()
	{
		Flow::Renderer::Submit(this);

		WorldComponent::Render();
	}

	void StaticMeshComponent::RefreshBinds()
	{
		VertexLayout MeshLayout;
		m_Binds = m_StaticMesh->GenerateBinds(MeshLayout);
		m_IndexBuffer = m_StaticMesh->GetIndexBuffer();

		AddBind(std::make_shared<TransformConstantBuffer>(this));
		m_Material->BindMaterial(this, MeshLayout);
	}
}