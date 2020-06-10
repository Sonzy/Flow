#include "Flowpch.h"
#include "SkyboxComponent.h"

#include "Flow\Rendering\Core\Bindables\Topology.h"
#include "Flow\Rendering\Core\Bindables\BindableVertexBuffer.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"
#include "Flow\Rendering\Core\Bindables\Rasterizer.h"
#include "Flow\Rendering\Core\Vertex\VertexBuffer.h"
#include "Flow/Rendering/Renderer.h"

#include "Flow/Assets/AssetSystem.h"

#include "Flow\Assets\Meshes\MeshAsset.h" 

SkyboxComponent::SkyboxComponent(const std::string& Name)
	: RenderableComponent(Name)
{
	AddBind(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	VertexLayout Layout;
	Layout.Append(ElementType::Position3D);
	Layout.Append(ElementType::Normal);
	Layout.Append(ElementType::Texture2D);

	// Define Vertex Buffer information
	VertexBuffer VBuffer(Layout);

	_Mesh = reinterpret_cast<MeshAsset*>(AssetSystem::GetAsset(_MeshPath));
	CHECK_RETURN(!_Mesh, "Skybox::Skybox: Failed to load mesh");
	_Material = AssetSystem::GetAsset<MaterialAsset>(_MaterialPath)->GetMaterial();
	CHECK_RETURN(!_Material, "Skybox::Skybox: Failed to load material");

	Mesh* m_Mesh = _Mesh->GetMesh(0);
	for (auto& Vertex : m_Mesh->GetVertices())
	{
		VBuffer.EmplaceBack( //TODO: Dont actually need to know whats in here, just need to know the stride and offsets
			DirectX::XMFLOAT3{ Vertex._Position.X ,  Vertex._Position.Y,  Vertex._Position.Z },
			DirectX::XMFLOAT3{ Vertex._Normal.X ,  Vertex._Normal.Y,  Vertex._Normal.Z },
			DirectX::XMFLOAT2{ Vertex._TexCoord.X,  Vertex._TexCoord.Y }
		);
	}

	std::vector<unsigned short> indices;
	indices.reserve(m_Mesh->GetNumFaces() * 3); //Using triangles, change for quads
	for (auto& Face : m_Mesh->GetFaces())
	{
		assert(Face._NumIndices == 3);
		indices.push_back(Face._Indices[0]);
		indices.push_back(Face._Indices[1]);
		indices.push_back(Face._Indices[2]);
	}

	//Add Vertex Buffer Bind
	AddBind(BindableVertexBuffer::Resolve(_MeshPath, VBuffer));

	_Material->BindMaterial(this, Layout);

	//Bind Index Buffer
	AddBind(IndexBuffer::Resolve(_MeshPath, indices));

	//Bind Transform
	AddBind(std::make_shared<TransformConstantBuffer>(this));

	AddBind(Rasterizer::Resolve(CullMode::Front));

	_SimulatePhysics = false;
}


DirectX::XMMATRIX SkyboxComponent::GetTransformXM() const
{
	DirectX::XMFLOAT3 CamPos = RenderCommand::GetMainCamera()->GetCameraPosition().ToDXFloat3();
	return DirectX::XMMatrixScaling(300.0f, 300.0f, 300.0f) *
		DirectX::XMMatrixTranslation(CamPos.x, CamPos.y, CamPos.z);
}

void SkyboxComponent::Render()
{
	Renderer::Submit(this);

	WorldComponent::Render();
}
