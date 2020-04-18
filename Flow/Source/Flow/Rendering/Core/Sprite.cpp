#include "Flowpch.h"
#include "Sprite.h"

#include "Flow\Assets\Textures\TextureAsset.h"

#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Vertex\VertexBuffer.h"
#include "Flow\Rendering\Core\Bindables\Topology.h"
#include "Flow\Rendering\Core\Bindables\BindableVertexBuffer.h"
#include "Flow\Rendering\Core\Bindables\Texture.h"
#include "Flow\Rendering\Core\Bindables\Sampler.h"
#include "Flow\Rendering\Core\Bindables\Shaders\PixelShader.h"
#include "Flow\Rendering\Core\Bindables\Shaders\VertexShader.h"
#include "Flow\Rendering\Core\Bindables\InputLayout.h"
#include "Flow\Rendering\Core\Bindables\Stencil.h"


#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\RenderAPI.h"


#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Shaders\ShaderAsset.h"

#include "Flow\GameFramework\Components\CameraComponent.h"

#include "ThirdParty\ImGui\imgui.h"

#include "Flow\Application.h"

#include "Flow\Rendering\Core\Bindables\Rasterizer.h"

namespace Flow
{
	Sprite::Sprite(TextureAsset* const Texture)
	{
		Texture_ = Texture;
		AddBind(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		VertexLayout Layout;
		Layout.Append(ElementType::Position2D);
		Layout.Append(ElementType::Texture2D);

		VertexBuffer VBuffer(Layout);
		VBuffer.EmplaceBack(DirectX::XMFLOAT2{-0.5f, -0.5f}, DirectX::XMFLOAT2{ 0.0f, 1.0f }); //TODO: Have flipped the texcoords in the y axis, not sure why the images are flipped in y so this is a quick fix
		VBuffer.EmplaceBack(DirectX::XMFLOAT2{0.5f,	-0.5f}, DirectX::XMFLOAT2{ 1.0f, 1.0f});
		VBuffer.EmplaceBack(DirectX::XMFLOAT2{-0.5f, 0.5f}, DirectX::XMFLOAT2{ 0.0f, 0.0f });
		VBuffer.EmplaceBack(DirectX::XMFLOAT2{0.5f, 0.5f}, DirectX::XMFLOAT2{ 1.0f, 0.0f});

		//VBuffer.EmplaceBack(DirectX::XMFLOAT2{ -0.5f, -0.5f }, DirectX::XMFLOAT2{ 0.0f, 0.0f });
		//VBuffer.EmplaceBack(DirectX::XMFLOAT2{ 0.5f,	-0.5f }, DirectX::XMFLOAT2{ 1.0f, 0.0f });
		//VBuffer.EmplaceBack(DirectX::XMFLOAT2{ -0.5f, 0.5f }, DirectX::XMFLOAT2{ 0.0f, 1.0f });
		//VBuffer.EmplaceBack(DirectX::XMFLOAT2{ 0.5f, 0.5f }, DirectX::XMFLOAT2{ 1.0f, 1.0f });

		std::vector<unsigned short> Indices =
		{
			0, 1, 2,
			2, 1, 3
		};
		//AddBind(std::make_shared<Stencil>(StencilMode::Mask, DepthMode::Off));
		AddBind(BindableVertexBuffer::Resolve("Sprite", VBuffer));

		AddBind(Texture::Resolve(Texture_, 0));
		AddBind(Sampler::Resolve());

		ShaderAsset* PShader = AssetSystem::GetAsset<ShaderAsset>("SpriteShaderP");
		ShaderAsset* VShader = AssetSystem::GetAsset<ShaderAsset>("SpriteShaderV");

		auto vShader = VertexShader::Resolve(VShader->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		AddBind(std::move(vShader));
		AddBind(PixelShader::Resolve(PShader->GetPath()));

		AddBind(InputLayout::Resolve(Layout, vShaderByteCode));
		AddBind(IndexBuffer::Resolve("Sprite", Indices));

		BoundVCB = std::make_shared<VertexConstantBuffer<SpriteMatrix>>(ConstBuffer, 3);
		Update();
		AddBind(BoundVCB);
	}

	void Sprite::Update()
	{
		const auto modelView = UseProjection ?
			GetTransformXM() * RenderCommand::DX11GetOrthographicMatrix() * RenderCommand::GetCamera().GetProjectionMatrix() :
			GetTransformXM() * RenderCommand::DX11GetOrthographicMatrix();

		ConstBuffer.ViewpMatrix = TransposeMatrix ? DirectX::XMMatrixTranspose(modelView) : modelView;
		BoundVCB->Update(ConstBuffer);
	}

	DirectX::XMMATRIX Sprite::GetTransformXM() const
	{
		IntVector2D Size = RenderCommand::GetWindowSize();
		
		FLOW_ENGINE_LOG("Height: {0} Drawing at: {1}, Pos.y: {2}", Size.X, (Size.Y / 2) + Position_.Y - (Scale_.Y / 2), Position_.Y);

		return DirectX::XMMatrixScaling(Scale_.X, Scale_.Y, Scale_.Z) *
			DirectX::XMMatrixRotationRollPitchYaw(Math::DegreesToRadians(Rotation_.Pitch), Math::DegreesToRadians(Rotation_.Yaw), Math::DegreesToRadians(Rotation_.Roll)) *
			DirectX::XMMatrixTranslation(-((int)Size.X / 2) + Position_.X + (Scale_.X / 2), (Size.Y / 2) + Position_.Y - (Scale_.Y / 2), 1.0f);
	}

	void Sprite::ControlWindow()
	{
		if (ImGui::Begin("SpriteTesting"))
		{
			ImGui::InputFloat3("Position", (float*)&Position_);
			ImGui::InputFloat3("Rotation", (float*)&Rotation_);
			ImGui::InputFloat3("Scale", (float*)&Scale_);

			ImGui::Checkbox("Transpose Matrix", &TransposeMatrix);
			ImGui::Checkbox("Use Projection", &UseProjection);
		}
		ImGui::End();
	}
}