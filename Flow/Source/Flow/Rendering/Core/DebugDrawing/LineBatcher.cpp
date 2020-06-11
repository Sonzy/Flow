#include "Flowpch.h"
#include "LineBatcher.h"

#include "Flow/Rendering/Core/Bindables/Topology.h"
#include "Flow/Rendering/Core/Bindables/InputLayout.h"
#include "Flow/Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Flow/Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Flow/Rendering/Core/Bindables/BindableVertexBuffer.h"

#include "Flow/Rendering/Core/Vertex/VertexLayout.h"
#include "Flow/Rendering/Core/Vertex/VertexBuffer.h"

#include "Flow/Assets/AssetSystem.h"

LineBatcher::LineBatcher()
	: _VertexBuffer(nullptr), _BindableVertexBuffer(nullptr), _VertexLayout(nullptr), _VertexCB(nullptr)
{

}

LineBatcher::~LineBatcher()
{
	delete _VertexBuffer;
	delete _VertexCB;
	delete _VertexLayout;
	//delete _BindableVertexBuffer; TODO: Crashes on release?
}

void LineBatcher::Initialise()
{
	AddBind(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_LINELIST));

	_VertexLayout = new VertexLayout();
	_VertexLayout->Append(ElementType::Position3D);
	_VertexLayout->Append(ElementType::Float3Colour);

	_VertexBuffer = new VertexBuffer(*_VertexLayout);

	AddBind(PixelShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("LineColourP")->GetPath()));
	auto vShader = VertexShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("LineColourV")->GetPath());
	auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
	AddBind(std::move(vShader));
	AddBind(InputLayout::Resolve(*_VertexLayout, vShaderByteCode));

	_VertexCB = new VertexConstantBuffer<ViewProjectionBuffer>(0);
}

void LineBatcher::AddLine(Vector From, Vector To, Vector Colour)
{
	_VertexBuffer->EmplaceBack(
		DirectX::XMFLOAT3{ From.X, From.Y, From.Z },
		DirectX::XMFLOAT3{ Colour.X, Colour.Y, Colour.Z });

	_VertexBuffer->EmplaceBack(
		DirectX::XMFLOAT3{ To.X, To.Y, To.Z },
		DirectX::XMFLOAT3{ Colour.X, Colour.Y, Colour.Z });

	Lines++;
}

void LineBatcher::DrawLines()
{
	if (Lines == 0)
		return;

	_BindableVertexBuffer = new BindableVertexBuffer("LineBatcher", *_VertexBuffer);

	_CameraMatrix._ViewProjectionMatrix = RenderCommand::GetMainCamera()->GetTransposedCachedViewProjection();
	_VertexCB->Update(_CameraMatrix);

	_VertexCB->Bind();
	_BindableVertexBuffer->Bind();
	BindAll();

	RenderCommand::Draw(Lines * 2);

	FlushLines();
}

void LineBatcher::FlushLines()
{
	//PROFILE_FUNCTION();

	delete _VertexBuffer;
	delete _BindableVertexBuffer;

	_VertexBuffer = new VertexBuffer(*_VertexLayout);
	Lines = 0;
}

void LineBatcher::BindAll()
{
	for (const auto& Bind : _Binds)
	{
		Bind->Bind();
	}
}

void LineBatcher::AddBind(std::shared_ptr<Bindable> NewBind)
{
	_Binds.push_back(std::move(NewBind));
}
