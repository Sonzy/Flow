#include "Flowpch.h"
#include "StaticMeshComponent.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"
#include "Flow\Rendering\Core\Bindables\Stencil.h"
#include "Flow\Rendering\Core\Bindables\Rasterizer.h"

#include "ThirdParty\ImGui\imgui.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "Flow\GameFramework\World.h"

namespace Flow
{
	StaticMeshComponent::StaticMeshComponent()
		: StaticMeshComponent("StaticMeshComponent")
	{
	}

	StaticMeshComponent::StaticMeshComponent(const std::string& Name, MeshAsset* Mesh, Material* Material, int MeshIndex)
		: RenderableComponent(Name), StaticMesh_(nullptr), Material_(nullptr)
	{
		if (Mesh && Material)
			SetMeshAndMaterial(Mesh, Material, MeshIndex);
	}

	StaticMeshComponent::~StaticMeshComponent()
	{
		delete StaticMesh_;
	}

	void StaticMeshComponent::InitialiseComponent(MeshAsset* Mesh, Material* Material)
	{
		SetMeshAndMaterial(Mesh, Material);
	}

	void StaticMeshComponent::SetMeshAndMaterial(MeshAsset* Mesh, Material* Material, int MeshIndex)
	{
		CHECK_RETURN(!Mesh, "StaticMeshComponent::SetMeshAndMaterial: Mesh was nullptr");
		CHECK_RETURN(!Material, "StaticMeshComponent::SetMeshAndMaterial: Material was nullptr");

		StaticMesh_ = Mesh->GetMesh(MeshIndex);
		Material_ = Material;

		RefreshBinds();
	}

	void StaticMeshComponent::SetStaticMesh(const std::string& MeshName)
	{
		//TODO: Change mesh at runtime without creating new object
		MeshAsset* NewAsset = AssetSystem::GetAsset<MeshAsset>(MeshName);
		CHECK_RETURN(!NewAsset, "StaticMeshComponent::SetStaticMesh: Failed to get new static mesh.");
		
		StaticMesh_ = NewAsset->GetMesh(0);

		RefreshBinds(); //TODO: Dont call this twice? dunno
	}

	void StaticMeshComponent::SetMaterial(Material* NewMaterial)
	{
		Material_ = NewMaterial;

		RefreshBinds();
	}

	void StaticMeshComponent::Render()
	{
		PROFILE_FUNCTION();

		Flow::Renderer::Submit(this);

		if (DrawOutline_)
			DrawOutline();

		WorldComponent::Render();
	}

	void StaticMeshComponent::RefreshBinds()
	{
		PROFILE_FUNCTION();

		VertexLayout MeshLayout;
		Binds_ = StaticMesh_->GenerateBinds(MeshLayout);
		IndexBuffer_ = StaticMesh_->GetIndexBuffer();

		auto Transform = std::make_shared<TransformConstantBuffer>(this);
		AddBind(Transform);
		Material_->BindMaterial(this, MeshLayout);

		AddBatchedBind(Rasterizer::Resolve(false));

		//TODO: Temp stencil testing
		if (DrawOutline_)
		{
			AddBatchedBind(std::make_shared<Stencil>(StencilMode::Write, DepthMode::On));
			VertexBuffer Dummy = VertexBuffer(MeshLayout);
			std::vector<unsigned short> Dummy2;

			OutlineEffect.push_back(BindableVertexBuffer::Resolve(StaticMesh_->Parent_->GetAssetName() + std::to_string(StaticMesh_->MeshIndex_), Dummy));
			OutlineEffect.push_back(IndexBuffer::Resolve(StaticMesh_->Parent_->GetAssetName() + std::to_string(StaticMesh_->MeshIndex_), Dummy2));

			auto VS = VertexShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("SolidColourVS")->GetPath());
			auto VSByteCode = static_cast<VertexShader&>(*VS).GetByteCode();
			OutlineEffect.push_back(std::move(VS));
			OutlineEffect.push_back(PixelShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("SolidColourPS")->GetPath()));
			struct SolidColorBuffer
			{
				DirectX::XMFLOAT4 color = { 1.0f,0.4f,0.4f,1.0f };
			} scb;
			OutlineEffect.push_back(PixelConstantBuffer<SolidColorBuffer>::Resolve(scb, 2u));

			OutlineEffect.push_back(InputLayout::Resolve(MeshLayout, VSByteCode));
			OutlineEffect.push_back(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			OutlineEffect.push_back(Transform);
			OutlineEffect.push_back(std::make_shared<Stencil>(StencilMode::Mask, DepthMode::Off));
		}
		else
			AddBatchedBind(std::make_shared<Stencil>(StencilMode::Off));
	}

	DirectX::XMMATRIX StaticMeshComponent::GetTransformXM()
	{
		Transform WorldTransform = GetWorldTransform();
		Rotator RadianRotation = Rotator::AsRadians(WorldTransform.Rotation_);

		Vector Scale = GetWorldScale();
		if (CurrentlyOutlining)
			Scale += OutlineThickness_;

		auto Trans = DirectX::XMMatrixScaling(Scale.X, Scale.Y, Scale.Z) *
			DirectX::XMMatrixRotationRollPitchYaw(RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll) *
			DirectX::XMMatrixTranslation(WorldTransform.Position_.X, WorldTransform.Position_.Y, WorldTransform.Position_.Z);

		return Trans;
	}

	void StaticMeshComponent::EnableOutlineDrawing(bool Enabled)
	{
		if (!StaticMesh_)
		{
			FLOW_ENGINE_ERROR("Tried to draw outline on component without mesh");
			return;
		}
		DrawOutline_ = Enabled;
		RefreshBinds();
	}

	void StaticMeshComponent::DrawOutline()
	{
		PROFILE_FUNCTION();

		CurrentlyOutlining = true;
		for (auto& b : OutlineEffect)
		{
			b->Bind();
		}
		RenderCommand::DrawIndexed(GetIndexBuffer().GetCount());
		CurrentlyOutlining = false;
	}

	void StaticMeshComponent::DrawDetailsWindow(bool bDontUpdate)
	{
		PROFILE_FUNCTION();

		RenderableComponent::DrawDetailsWindow(bDontUpdate);

		bool BindsDirty = false;
		BindsDirty |= ImGui::Checkbox("Outline", &DrawOutline_);
		BindsDirty |= ImGui::InputFloat("Outline Scale", &OutlineThickness_, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue);

		if (BindsDirty)
			RefreshBinds();
	}

	bool StaticMeshComponent::CreateCollision()
	{
		delete Collision_;

		btConvexHullShape* Shape = new btConvexHullShape();
		auto Vertices = StaticMesh_->GetCollisionVertices();
		for (auto Vert : Vertices)
		{
			btVector3 btv = btVector3(Vert.X, Vert.Y, Vert.Z);
			Shape->addPoint(btv);
		}

		Collision_ = Shape;
		Collision_->setMargin(0.04f);

		Transform trans = GetWorldTransform();
		Collision_->setLocalScaling(btVector3(trans.Scale_.X, trans.Scale_.Y, trans.Scale_.Z));

		return true;
	}

	void StaticMeshComponent::BindBatchables()
	{
		BindBatchedBinds();
	}

	void StaticMeshComponent::BindNonBatchables()
	{
		BindNonBatchables();
	}
}