//= Includes ==========================================

#include "Flowpch.h"
#include "RenderQueue.h"

#include "Rendering/Core/Bindables/Stencil.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"
#include "Rendering/Core/Bindables/Shaders/NullPixelShader.h"
#include "Rendering/Core/Bindables/Rasterizer.h"
#include "Rendering/Other/DepthBuffer.h"

#if WITH_EDITOR
#include "Editor/Editor.h"
#include "Rendering/Core/TemplateBuffers/TemplateBuffers.h"
#endif


FrameBuffer* RenderQueue::sm_SelectionBuffer = nullptr;
RenderPass	RenderQueue::sm_CurrentPass = RenderPass::Main;
std::unordered_map<RenderPass, Pass> RenderQueue::sm_Passes =
{
	{ RenderPass::Main, Pass() },
	{ RenderPass::OutlineMasking, Pass() },
	{ RenderPass::Outline, Pass() },
	{ RenderPass::NoDepth, Pass() },
	{ RenderPass::Standard2D, Pass() },
	{ RenderPass::UI, Pass() },
	{ RenderPass::Selection, Pass() },
	{ RenderPass::Selection2D, Pass() },
};

RenderQueue::RenderQueue()
{
}

RenderQueue::~RenderQueue()
{
	//TODO: canny remember if this one destructs.
	sm_Passes.clear();

	delete sm_SelectionBuffer;
}

Pass& RenderQueue::GetPass(RenderPass pass)
{
	return sm_Passes[pass];
}

void RenderQueue::SubmitTechnique(Technique* Tech)
{
}

void RenderQueue::AcceptJob(Job NewJob, RenderPass TargetPass)
{
	//TODO: Ensure that this is a valid pass
	sm_Passes[TargetPass].Accept(NewJob);
}

void RenderQueue::Execute()
{
	//= Main Render pass

	Pass& mainPass = sm_Passes[RenderPass::Main];
	if (mainPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::Main;

		RenderCommand::SetPerspective();
		Rasterizer::Resolve(CullMode::Back)->Bind();
		Stencil::Resolve(StencilMode::Off)->Bind();

		mainPass.Execute();
	}

	Pass& outlineMaskPass = sm_Passes[RenderPass::OutlineMasking];
	if (outlineMaskPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::OutlineMasking;

		Stencil::Resolve(StencilMode::Write)->Bind();
		NullPixelShader::Resolve()->Bind(); //Stop D3D11 from using render targets

		outlineMaskPass.Execute();
	}

	Pass& outlinePass = sm_Passes[RenderPass::Outline];
	if (mainPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::Outline;

		Vector3 Colour = Editor::GetSettings().m_ObjectHighlightColour;

		//= Outline Drawing Pass.
		Stencil::Resolve(StencilMode::Mask)->Bind();

		CBT_Colour ColourBuffer = CBT_Colour(Colour.x, Colour.y, Colour.z, 1.0f);
		auto PXCB = static_cast<PixelConstantBuffer<CBT_Colour>*>(PixelConstantBuffer<CBT_Colour>::Resolve(ColourBuffer, 2u));
		PXCB->Update(ColourBuffer);
		PXCB->Bind();

		outlinePass.Execute();
	}

	Pass& noDepthPass = sm_Passes[RenderPass::NoDepth];
	if (mainPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::NoDepth;
		Stencil::Resolve(StencilMode::NoDepth)->Bind();

		noDepthPass.Execute();
	}

	Stencil::Resolve(StencilMode::Off)->Bind();

	Pass& standard2DPass = sm_Passes[RenderPass::Standard2D];
	if (standard2DPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::Standard2D;

		RenderCommand::SetOrthographic();
		Stencil::Resolve(StencilMode::Off)->Bind();

		standard2DPass.Execute();
	}

	Pass& UIPass = sm_Passes[RenderPass::UI];
	if (UIPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::UI;

		RenderCommand::SetOrthographic();
		Stencil::Resolve(StencilMode::NoDepth)->Bind();

		UIPass.Execute();
	}
	Stencil::Resolve(StencilMode::Off)->Bind();

	Pass& frontCullPass = sm_Passes[RenderPass::FrontFaceCulling];
	if (frontCullPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::FrontFaceCulling;

		RenderCommand::SetPerspective();
		Rasterizer::Resolve(CullMode::Front)->Bind();

		frontCullPass.Execute();
	}
	Rasterizer::Resolve(CullMode::Back)->Bind();

	bool selectionBufferClearedThisFrame = false;

	RenderCommand::SetClearColour(0.0f, 0.0f, 0.0f, 1.0f);

	Pass& selectionPass = sm_Passes[RenderPass::Selection];
	if (selectionPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::Selection;

		if (sm_SelectionBuffer == nullptr)
		{
			sm_SelectionBuffer = new FrameBuffer(RenderCommand::GetWindowSize().x, RenderCommand::GetWindowSize().y, true);
		}

		if (sm_SelectionBuffer->GetWidth() != RenderCommand::GetWindowSize().x || sm_SelectionBuffer->GetHeight() != RenderCommand::GetWindowSize().y)
		{
			sm_SelectionBuffer->Resize(RenderCommand::GetWindowSize().x, RenderCommand::GetWindowSize().y);
		}

		RenderCommand::BindFrameBuffer(sm_SelectionBuffer);
		selectionBufferClearedThisFrame = true;

		RenderCommand::SetPerspective();

		selectionPass.Execute();

		// Post Selection Rendering
		RenderCommand::BindEditorBufferWithoutClear();
	}

	Pass& selectionPass2D = sm_Passes[RenderPass::Selection2D];
	if (selectionPass2D.IsEnabled())
	{
		sm_CurrentPass = RenderPass::Selection2D;

		if (sm_SelectionBuffer == nullptr)
		{
			sm_SelectionBuffer = new FrameBuffer(RenderCommand::GetWindowSize().x, RenderCommand::GetWindowSize().y, true);
		}

		if (sm_SelectionBuffer->GetWidth() != RenderCommand::GetWindowSize().x || sm_SelectionBuffer->GetHeight() != RenderCommand::GetWindowSize().y)
		{
			sm_SelectionBuffer->Resize(RenderCommand::GetWindowSize().x, RenderCommand::GetWindowSize().y);
		}

		selectionBufferClearedThisFrame ? RenderCommand::BindFrameBufferWithoutClear(sm_SelectionBuffer) : RenderCommand::BindFrameBuffer(sm_SelectionBuffer);
		selectionBufferClearedThisFrame = true;

		RenderCommand::SetOrthographic();
		Stencil::Resolve(StencilMode::NoDepth)->Bind();

		selectionPass2D.Execute();

		// Post Selection Rendering
		RenderCommand::BindEditorBufferWithoutClear();
	}

	Stencil::Resolve(StencilMode::Off)->Bind();

	//Reset for late rendering (imgui etc)
	RenderCommand::SetPerspective();

	sm_CurrentPass = RenderPass::Main;
}

void RenderQueue::Reset()
{	
	for (auto& Pass : sm_Passes)
	{
		Pass.second.Reset();
	}
}

RenderPass RenderQueue::GetActiveRenderPass()
{
	return sm_CurrentPass;
}
