#include "Flowpch.h"
#include "Skybox.h"

#include "Flow\Helper\HelperMacros.h"
#include "Flow\Logging\Log.h"

#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"

#include "Flow\Rendering\Core\Bindables\Topology.h"
#include "Flow\Rendering\Core\Bindables\BindableVertexBuffer.h"
#include "Flow\Rendering\Core\Materials\Material.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"
#include "Flow\Rendering\Core\Bindables\Rasterizer.h"

#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Vertex\VertexBuffer.h"

#include "Flow\Rendering\Core\Camera\Camera.h"
#include "Flow\GameFramework\Components\CameraComponent.h"


namespace Flow
{
	Skybox::Skybox()
	{
		AddBind(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		// Define Vertex Layout
		VertexLayout Layout;
		Layout.Append(ElementType::Position3D);
		Layout.Append(ElementType::Normal);
		Layout.Append(ElementType::Texture2D);

		// Define Vertex Buffer information
		VertexBuffer VBuffer(Layout);

		Mesh_ = reinterpret_cast<MeshAsset*>(AssetSystem::GetAsset(MeshPath_));
		CHECK_RETURN(!Mesh_, "Skybox::Skybox: Failed to load mesh");
		Material_ = AssetSystem::GetAsset<MaterialAsset>(MaterialPath_)->GetMaterial();
		CHECK_RETURN(!Material_, "Skybox::Skybox: Failed to load material");

		Mesh* m_Mesh = Mesh_->GetMesh(0);
		for (auto& Vertex : m_Mesh->GetVertices())
		{
			VBuffer.EmplaceBack( //TODO: Dont actually need to know whats in here, just need to know the stride and offsets
				DirectX::XMFLOAT3{ Vertex.Position_.X ,  Vertex.Position_.Y,  Vertex.Position_.Z },
				DirectX::XMFLOAT3{ Vertex.Normal_.X ,  Vertex.Normal_.Y,  Vertex.Normal_.Z },
				DirectX::XMFLOAT2{ Vertex.TexCoord_.X,  Vertex.TexCoord_.Y }
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(m_Mesh->GetNumFaces() * 3); //Using triangles, change for quads
		for (auto& Face : m_Mesh->GetFaces())
		{
			assert(Face.NumIndices_ == 3);
			indices.push_back(Face.Indices_[0]);
			indices.push_back(Face.Indices_[1]);
			indices.push_back(Face.Indices_[2]);
		}

		//Add Vertex Buffer Bind
		AddBind(BindableVertexBuffer::Resolve(MeshPath_, VBuffer));

		Material_->BindMaterial(this, Layout);

		//Bind Index Buffer
		AddBind(IndexBuffer::Resolve(MeshPath_, indices));

		//Bind Transform
		AddBind(std::make_shared<TransformConstantBuffer>(this));

		AddBind(Rasterizer::Resolve(true));
	}

	DirectX::XMMATRIX Skybox::GetTransformXM() const
	{
		DirectX::XMFLOAT3 CamPos = RenderCommand::GetCamera().GetWorldPosition().ToDXFloat3();
		return DirectX::XMMatrixScaling(300.0f, 300.0f, 300.0f) *
			DirectX::XMMatrixTranslation(CamPos.x, CamPos.y, CamPos.z);
	}
}