#include "Flowpch.h"
#include "Line.h"

#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Shaders\ShaderAsset.h"

#include "Flow/Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Flow/Rendering/Core/Bindables/Shaders/PixelShader.h"

#include "Flow\GameFramework\Components\CameraComponent.h"

namespace Flow
{
	VertexLayout Line::VertexLayout_ = VertexLayout();
	std::vector<std::shared_ptr<Bindable>> Line::Binds_ = std::vector<std::shared_ptr<Bindable>>();
	IndexBuffer* Line::IndexBuffer_ = nullptr;
	int Line::Count = 0;

	Line::Line()
	{
	}

	void Line::Initialise()
	{
		Line* LineObj = new Line();
		LineObj->AddBind(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_LINELIST));

		VertexLayout_.Append(ElementType::Position3D);

		std::vector<unsigned short> indices;
		indices.push_back(0);
		indices.push_back(1);
		//Bind Index Buffer
		LineObj->AddBind(IndexBuffer::Resolve("Line", indices));

		//Material
		LineObj->AddBind(PixelShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("LineShaderP")->GetPath()));
		auto vShader = VertexShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("LineShaderV")->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		LineObj->AddBind(std::move(vShader));
		LineObj->AddBind(InputLayout::Resolve(VertexLayout_, vShaderByteCode));
	}

	void Line::DrawLine(Vector From, Vector To, Vector Colour)
	{
		Line NewLine = Line();
		
		// Define Vertex Layout
		VertexBuffer VBuffer(VertexLayout_);
		VBuffer.EmplaceBack(DirectX::XMFLOAT3{ From.X ,  From.Y,  From.Z });
		VBuffer.EmplaceBack(DirectX::XMFLOAT3{ To.X ,  To.Y,  To.Z });
		
		//Add Vertex Buffer Bind
		BindableVertexBuffer BVB = BindableVertexBuffer("", VBuffer);
		
		struct LineColour
		{
			DirectX::XMFLOAT4 Colour;
		} LineColourCBuffer;
		LineColourCBuffer = { { Colour.X, Colour.Y, Colour.Z, 1.0f } };
		auto ColourBuffer = PixelConstantBuffer<LineColour>(LineColourCBuffer, 3u);
		
		
		//Initialise VertexConstBuffer
		struct LineTransform
		{
			DirectX::XMMATRIX ViewProjectionMatrix = DirectX::XMMatrixTranspose(RenderCommand::GetCamera().GetViewMatrix() * RenderCommand::GetCamera().GetProjectionMatrix());
		} Trans;
		
		std::shared_ptr<VertexConstantBuffer<LineTransform>> VCB = std::make_shared<VertexConstantBuffer<LineTransform>>(0);
		VCB->Update(Trans);


		VCB->Bind();
		BVB.Bind();
		ColourBuffer.Bind();


		NewLine.BindAll();
		
		RenderCommand::DrawIndexed(IndexBuffer_->GetCount());
				
		Count++;
	}

	DirectX::XMMATRIX Line::GetTransformXM() const
	{
		return DirectX::XMMATRIX();
	}

	void Line::AddBind(std::shared_ptr<Bindable> bind)
	{
		//If index buffer, only allow single bind.
		if (typeid(*bind) == typeid(IndexBuffer))
		{
			assert("Renderable::AddBind: Cannot bind multiple index buffers." && IndexBuffer_ == nullptr);
			IndexBuffer_ = static_cast<IndexBuffer*>(bind.get());
		}

		Binds_.push_back(std::move(bind));
	}

	void Line::BindAll()
	{
		for (auto& b : Binds_)
		{
			b->Bind();
		}
	}
}