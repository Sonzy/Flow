#include "Flowpch.h"
#include "LineBatcher.h"

#include "Bindables/BindableVertexBuffer.h"
#include "Bindables/IndexBuffer.h"
#include "Bindables/Sampler.h"
#include "Bindables\Texture.h"
#include "Bindables/InputLayout.h"
#include "Bindables/Topology.h"
#include "Bindables/Shaders/VertexShader.h"
#include "Bindables/Shaders/PixelShader.h"
#include "Bindables/ConstantBuffers/ConstantBuffer.h"
#include "Bindables/ConstantBuffers/ShaderConstantBuffers.h"
#include "Bindables/ConstantBuffers/TransformConstantBuffer.h"

#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Shaders\ShaderAsset.h"

#include "Flow\Rendering\Core\Camera\Camera.h"
#include "Flow\GameFramework\Components\CameraComponent.h"

namespace Flow
{
	LineBatcher::LineBatcher()
		: VertexBuffer_(nullptr), BVertexBuffer_(nullptr), 
		Layout_(nullptr), VConstantBuffer_(nullptr)
	{

	}

	LineBatcher::~LineBatcher()
	{
		delete VertexBuffer_;
		delete VConstantBuffer_;
		//delete BVertexBuffer_; //TODO: only deleted at program end, but i should
								//do this properly
		delete Layout_;
	}

	void LineBatcher::Initialise()
	{
		AddBind(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_LINELIST));

		Layout_ = new VertexLayout();
		Layout_->Append(ElementType::Position3D);
		Layout_->Append(ElementType::Float3Colour);

		VertexBuffer_ = new VertexBuffer(*Layout_);

		AddBind(PixelShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("LineColourP")->GetPath()));
		auto vShader = VertexShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("LineColourV")->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		AddBind(std::move(vShader));
		AddBind(InputLayout::Resolve(*Layout_, vShaderByteCode));

		VConstantBuffer_ = new VertexConstantBuffer<ViewProjBuffer>(0);
	}

	void LineBatcher::AddLine(Vector From, Vector To, Vector Colour)
	{
		VertexBuffer_->EmplaceBack(
			DirectX::XMFLOAT3{ From.X, From.Y, From.Z },
			DirectX::XMFLOAT3{ Colour.X, Colour.Y, Colour.Z }	);

		VertexBuffer_->EmplaceBack(
			DirectX::XMFLOAT3{ To.X, To.Y, To.Z },
			DirectX::XMFLOAT3{ Colour.X, Colour.Y, Colour.Z });

		//Indices_.push_back(0);
		//Indices_.push_back(1);
		//Indices_.push_back(Lines * 2);
		//Indices_.push_back((Lines * 2) + 1);

		Lines++;
	}

	void LineBatcher::Draw()
	{
		if (Lines == 0)
			return;

		//AddBind(IndexBuffer::Resolve("Line", Indices_));
		//IBuffer_ = new IndexBuffer("LineBatcher", Indices_); //Lazy - IndexBuffer_ is const
		//IndexBuffer_ = IBuffer_;
		//IBuffer_->Bind();
		

		BVertexBuffer_ = new BindableVertexBuffer("LineBatcher", *VertexBuffer_);

		CameraMatrix.ViewProjectionMatrix_ = RenderCommand::GetCamera().GetTransposedCachedViewProjectionMatrix();
		VConstantBuffer_->Update(CameraMatrix);

		VConstantBuffer_->Bind();
		BVertexBuffer_->Bind();
		BindAll();
		
		//RenderCommand::DrawIndexed(IndexBuffer_->GetCount());
		RenderCommand::Draw(Lines * 2);

		FlushLines();
	}

	void LineBatcher::FlushLines()
	{
		PROFILE_FUNCTION();

		delete VertexBuffer_;
		delete BVertexBuffer_;
		delete IBuffer_;

		VertexBuffer_ = new VertexBuffer(*Layout_);
		Lines = 0;
	}
}