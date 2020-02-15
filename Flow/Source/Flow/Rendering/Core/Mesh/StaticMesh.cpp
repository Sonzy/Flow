#include "Flowpch.h"
#include "StaticMesh.h"
#include "Flow/Rendering/Core/Vertex/VertexLayout.h"
#include "Flow/Rendering/Core/Vertex/VertexBuffer.h"

#include "Flow\Application.h"

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Assets\Textures\TextureAsset.h"


namespace Flow
{
	StaticMesh::StaticMesh(const std::string& LocalPath)
	{
		if (!StaticInitialised())
		{
			// Define Vertex Layout
			VertexLayout Layout;
			Layout.Append(ElementType::Position3D);
			Layout.Append(ElementType::Texture2D);

			// Define Vertex Buffer information
			VertexBuffer VBuffer(Layout);

			MeshAsset* LoadedMesh = reinterpret_cast<MeshAsset*>(AssetSystem::GetAsset(LocalPath));
			CHECK_RETURN(!LoadedMesh, "StaticMesh::StaticMesh: Failed to get asset ({0})", LocalPath);

			for (auto& Vertex : LoadedMesh->GetVertices())
			{
				VBuffer.EmplaceBack(
					DirectX::XMFLOAT3{ Vertex.VertexPosition.X ,  Vertex.VertexPosition.Y,  Vertex.VertexPosition.Z },
					DirectX::XMFLOAT2 { Vertex.TextureCoord.X,  Vertex.TextureCoord.Y}
				);
			}

			std::vector<unsigned short> indices;
			indices.reserve(LoadedMesh->GetNumFaces() * 3); //Using triangles, change for quads
			for (auto& Face : LoadedMesh->GetFaces())
			{
				assert(Face.m_NumIndices == 3);
				indices.push_back(Face.m_Indices[0]);
				indices.push_back(Face.m_Indices[1]);
				indices.push_back(Face.m_Indices[2]);
			}

			//Add Vertex Buffer Bind
			AddStaticBindable(std::make_unique<BindableVertexBuffer>(VBuffer));

			AddStaticBindable(std::make_unique<Texture>(AssetSystem::GetAsset<TextureAsset>("Flow\\Assets\\Textures\\CharacterTexture.png")));
			AddStaticBindable(std::make_unique<Sampler>());

			std::wstring Local = Application::GetApplication().GetLocalFilePathWide();
			//Bind Shaders
			auto vShader = std::make_unique<VertexShader>(Local + L"Flow/Source/Flow/Rendering/Core/Shaders/TextureVS.cso");
			auto vShaderByteCode = vShader->GetByteCode();
			AddStaticBindable(std::move(vShader));
			AddStaticBindable(std::make_unique<PixelShader>(Local + L"Flow/Source/Flow/Rendering/Core/Shaders/TexturePS.cso"));

			//Bind Index Buffer
			AddStaticIndexBuffer(std::make_unique<IndexBuffer>(indices));

			//Bind Input Layout
			AddStaticBindable(std::make_unique<InputLayout>(VBuffer.GetLayout().GetD3DLayout(), vShaderByteCode));

			//Bind Topology
			AddStaticBindable (std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
			SetIndexFromStatic();
			
		//Bind Transform
		AddBind(std::make_unique<TransformConstantBuffer>(this));
	}



	DirectX::XMMATRIX StaticMesh::GetTransformXM() const
	{
		return DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.Pitch, m_Rotation.Yaw, m_Rotation.Roll) * //Rotate around box centre
			DirectX::XMMatrixTranslation(m_Position.X, m_Position.Y, m_Position.Z) * //Move relative to origin
			DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f); //Rotate around world centre
	}
}