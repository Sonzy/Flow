//= Includes ==========================================

#include "Flowpch.h"
#include "RenderQueue.h"
#include "Rendering/Core/RenderQueue/Pass.h"

#include "Rendering/Core/Bindables/Stencil.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"
#include "Rendering/Core/Bindables/Shaders/NullPixelShader.h"
#include "Rendering/Core/Bindables/Rasterizer.h"
#include "Rendering/Other/DepthBuffer.h"

#if WITH_EDITOR
#include "Editor/Editor.h"
#include "Rendering/Core/TemplateBuffers/TemplateBuffers.h"
#endif

RenderQueue* RenderQueue::sm_RenderQueue = new RenderQueue();

RenderQueue::RenderQueue()
{
	m_Passes.emplace_back(new Pass()); //Main Pass (Back Culled)
	m_Passes.emplace_back(new Pass()); //Main Pass (Forward Culled)
	m_Passes.emplace_back(new Pass()); //Main Pass (Two Sided)
	m_Passes.emplace_back(new Pass()); //Mask Pass
	m_Passes.emplace_back(new Pass()); //Outline Draw Pass
	m_Passes.emplace_back(new Pass()); //No Depth
	m_Passes.emplace_back(new Pass()); //2D
	m_Passes.emplace_back(new Pass()); //User Interface
}

RenderQueue::~RenderQueue()
{
	//TODO: canny remember if this one destructs.
	m_Passes.clear();
}

void RenderQueue::SubmitTechnique(Technique* Tech)
{
}

void RenderQueue::AcceptJob(Job NewJob, size_t TargetPass)
{
	//TODO: Ensure that this is a valid pass
	sm_RenderQueue->m_Passes[TargetPass]->Accept(NewJob);
}

void RenderQueue::Execute()
{
	RenderQueue* Queue = RenderQueue::Get();

	//= Main Render pass

	if (Queue->m_Pass0Enabled)
	{
		Rasterizer::Resolve(CullMode::Back)->Bind();
		Stencil::Resolve(StencilMode::Off)->Bind();
		Queue->m_Passes[0]->Execute();
	}

	//= Main Pass (Front Culled)
	if (Queue->m_Pass1Enabled)
	{
		Rasterizer::Resolve(CullMode::Front)->Bind();
		Queue->m_Passes[1]->Execute();
	}


	//= Main Pass (Two Sided
	if (Queue->m_Pass2Enabled)
	{
		Rasterizer::Resolve(CullMode::None)->Bind();
		Queue->m_Passes[2]->Execute();
	}

	//Reset 
	Rasterizer::Resolve(CullMode::Back)->Bind();
	//= Outline Masking pass
	if (Queue->m_Pass3Enabled)
	{
		Stencil::Resolve(StencilMode::Write)->Bind();
		NullPixelShader::Resolve()->Bind(); //Stop D3D11 from using render targets
		Queue->m_Passes[3]->Execute();
	}

	//TODO: Editor defs
	if (Queue->m_Pass4Enabled)
	{
		Vector3 Colour = Editor::GetSettings().m_ObjectHighlightColour;

		//= Outline Drawing Pass.
		Stencil::Resolve(StencilMode::Mask)->Bind();

		CBT_Colour ColourBuffer = CBT_Colour(Colour.x, Colour.y, Colour.z, 1.0f);
		auto PXCB = static_cast<PixelConstantBuffer<CBT_Colour>*>(PixelConstantBuffer<CBT_Colour>::Resolve(ColourBuffer, 2u));
		PXCB->Update(ColourBuffer);
		PXCB->Bind();

		Queue->m_Passes[4]->Execute();
	}

	//=  No depth pass
	if (Queue->m_Pass5Enabled)
	{
		Stencil::Resolve(StencilMode::Write)->Bind();
		Rasterizer::Resolve(CullMode::Back)->Bind();
		Queue->m_Passes[5]->Execute();
	}

	//= 2D Rendering =
	if (Queue->m_Pass6Enabled)
	{
		Queue->m_Passes[6]->Execute();
	}

	//= User Interface = 
	if (Queue->m_Pass7Enabled)
	{
		Queue->m_Passes[7]->Execute();
	}
}

RenderQueue* RenderQueue::Get()
{
	return sm_RenderQueue;
}

void RenderQueue::Reset()
{
	RenderQueue* Queue = RenderQueue::Get();
	
	for (auto& Pass : Queue->m_Passes)
	{
		Pass->Reset();
	}
}
