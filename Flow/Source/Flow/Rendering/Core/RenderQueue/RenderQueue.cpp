#include "Flowpch.h"
#include "RenderQueue.h"
#include "Flow/Rendering/Core/RenderQueue/Pass.h"

#include "Flow/Rendering/Core/Bindables/Stencil.h"
#include "Flow/Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"
#include "Flow/Rendering/Core/Bindables/Shaders/NullPixelShader.h"
#include "Flow/Rendering/Core/Bindables/Rasterizer.h"

#if WITH_EDITOR
#include "Flow/Layers/EditorLayer.h"
#include "Flow/Rendering/Core/TemplateBuffers/TemplateBuffers.h"
#endif

RenderQueue* RenderQueue::s_RenderQueue = new RenderQueue();

RenderQueue::RenderQueue()
{
	_Passes.emplace_back(new Pass()); //Main Pass (Back Culled)
	_Passes.emplace_back(new Pass()); //Main Pass (Forward Culled)
	_Passes.emplace_back(new Pass()); //Main Pass (Two Sided)
	_Passes.emplace_back(new Pass()); //Mask Pass
	_Passes.emplace_back(new Pass()); //Outline Draw Pass
}

RenderQueue::~RenderQueue()
{
	//TODO: canny remember if this one destructs.
	_Passes.clear();
}

void RenderQueue::SubmitTechnique(Technique* Tech)
{
}

void RenderQueue::AcceptJob(Job NewJob, size_t TargetPass)
{
	//TODO: Ensure that this is a valid pass
	s_RenderQueue->_Passes[TargetPass]->Accept(NewJob);
}

void RenderQueue::Execute()
{
	RenderQueue* Queue = RenderQueue::Get();

	//= Main Render pass

	if (Queue->_Pass0Enabled)
	{
		Rasterizer::Resolve(CullMode::Back)->Bind();
		Stencil::Resolve(StencilMode::Off)->Bind();
		Queue->_Passes[0]->Execute();
	}

	//= Main Pass (Front Culled)
	if (Queue->_Pass1Enabled)
	{
		Rasterizer::Resolve(CullMode::Front)->Bind();
		Queue->_Passes[1]->Execute();
	}


	//= Main Pass (Two Sided
	if (Queue->_Pass2Enabled)
	{
		Rasterizer::Resolve(CullMode::None)->Bind();
		Queue->_Passes[2]->Execute();
	}

	//Reset 
	Rasterizer::Resolve(CullMode::Back)->Bind();
	//= Outline Masking pass
	if (Queue->_Pass3Enabled)
	{
		Stencil::Resolve(StencilMode::Write)->Bind();
		NullPixelShader::Resolve()->Bind(); //Stop D3D11 from using render targets
		Queue->_Passes[3]->Execute();
	}

	//TODO: Editor defs
	if (Queue->_Pass4Enabled)
	{
		Vector Colour = EditorLayer::GetEditorSettings()._ObjectHighlightColour;

		//= Outline Drawing Pass.
		Stencil::Resolve(StencilMode::Mask)->Bind();

		CBT_Colour ColourBuffer = CBT_Colour(Colour.X, Colour.Y, Colour.Z, 1.0f);
		auto PXCB = std::static_pointer_cast<PixelConstantBuffer<CBT_Colour>>(PixelConstantBuffer<CBT_Colour>::Resolve(ColourBuffer, 2u));
		PXCB->Update(ColourBuffer);
		PXCB->Bind();

		Queue->_Passes[4]->Execute();
	}
}

RenderQueue* RenderQueue::Get()
{
	return s_RenderQueue;
}

void RenderQueue::Reset()
{
	RenderQueue* Queue = RenderQueue::Get();
	
	for (auto& Pass : Queue->_Passes)
	{
		Pass->Reset();
	}
}