//= Includes ==========================================

#include "pch.h"
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

	//Testing, main window background color
	Renderer::SetClearColour(0.25f, 0.0f, 0.25f, 1.0f);

	Pass& mainPass = sm_Passes[RenderPass::Main];
	if (mainPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::Main;

		Renderer::SetPerspective();
		Rasterizer::Resolve(Rasterizer::Cull_Back)->Bind();
		Bindables::Stencil::Resolve(Bindables::Stencil::Mode::Off)->Bind();

		mainPass.Execute();
	}

	Pass& outlineMaskPass = sm_Passes[RenderPass::OutlineMasking];
	if (outlineMaskPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::OutlineMasking;

		Bindables::Stencil::Resolve(Bindables::Stencil::Mode::Write)->Bind();
		NullPixelShader::Resolve()->Bind(); //Stop D3D11 from using render targets

		outlineMaskPass.Execute();
	}

#if WITH_EDITOR

	Pass& outlinePass = sm_Passes[RenderPass::Outline];
	if (mainPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::Outline;

		Vector3 Colour = Editor::GetSettings().m_ObjectHighlightColour;

		//= Outline Drawing Pass.
		Bindables::Stencil::Resolve(Bindables::Stencil::Mode::Mask)->Bind();

		CBT_Colour ColourBuffer = CBT_Colour(Colour.x, Colour.y, Colour.z, 1.0f);
		auto PXCB = static_cast<PixelConstantBuffer<CBT_Colour>*>(PixelConstantBuffer<CBT_Colour>::Resolve(ColourBuffer, 2u));
		PXCB->Update(ColourBuffer);
		PXCB->Bind();

		outlinePass.Execute();
	}

#endif // WITH_EDITOR

	Pass& noDepthPass = sm_Passes[RenderPass::NoDepth];
	if (mainPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::NoDepth;
		Bindables::Stencil::Resolve(Bindables::Stencil::Mode::NoDepth)->Bind();

		noDepthPass.Execute();
	}

	Bindables::Stencil::Resolve(Bindables::Stencil::Mode::Off)->Bind();

	Pass& standard2DPass = sm_Passes[RenderPass::Standard2D];
	if (standard2DPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::Standard2D;

		Renderer::SetOrthographic();
		Bindables::Stencil::Resolve(Bindables::Stencil::Mode::Off)->Bind();

		standard2DPass.Execute();
	}

	Pass& UIPass = sm_Passes[RenderPass::UI];
	if (UIPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::UI;

		Renderer::SetOrthographic();
		Bindables::Stencil::Resolve(Bindables::Stencil::Mode::NoDepth)->Bind();

		UIPass.Execute();
	}
	Bindables::Stencil::Resolve(Bindables::Stencil::Mode::Off)->Bind();

	Pass& frontCullPass = sm_Passes[RenderPass::FrontFaceCulling];
	if (frontCullPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::FrontFaceCulling;

		Renderer::SetPerspective();
		Rasterizer::Resolve(Rasterizer::Cull_Front)->Bind();

		frontCullPass.Execute();
	}
	Rasterizer::Resolve(Rasterizer::Cull_Back)->Bind();

#if WITH_EDITOR

	//Make selection background black
	Renderer::SetClearColour(0.0f, 0.0f, 0.0f, 1.0f);
	bool selectionBufferClearedThisFrame = false;

	Pass& selectionPass = sm_Passes[RenderPass::Selection];
	if (selectionPass.IsEnabled())
	{
		sm_CurrentPass = RenderPass::Selection;

		if (sm_SelectionBuffer == nullptr)
		{
			sm_SelectionBuffer = new FrameBuffer("Selection Buffer", Renderer::GetWindowSize().x, Renderer::GetWindowSize().y, true);
		}

		if (sm_SelectionBuffer->GetWidth() != Renderer::GetWindowSize().x || sm_SelectionBuffer->GetHeight() != Renderer::GetWindowSize().y)
		{
			sm_SelectionBuffer->Resize(Renderer::GetWindowSize().x, Renderer::GetWindowSize().y);
		}

		Renderer::BindFrameBuffer(sm_SelectionBuffer);
		selectionBufferClearedThisFrame = true;

		Renderer::SetPerspective();

		selectionPass.Execute();
	}

	Pass& selectionPass2D = sm_Passes[RenderPass::Selection2D];
	if (selectionPass2D.IsEnabled())
	{
		sm_CurrentPass = RenderPass::Selection2D;

		if (sm_SelectionBuffer == nullptr)
		{
			sm_SelectionBuffer = new FrameBuffer("Selection Buffer", Renderer::GetWindowSize().x, Renderer::GetWindowSize().y, true);
		}

		if (sm_SelectionBuffer->GetWidth() != Renderer::GetWindowSize().x || sm_SelectionBuffer->GetHeight() != Renderer::GetWindowSize().y)
		{
			sm_SelectionBuffer->Resize(Renderer::GetWindowSize().x, Renderer::GetWindowSize().y);
		}

		selectionBufferClearedThisFrame ? Renderer::BindFrameBufferWithoutClear(sm_SelectionBuffer) : Renderer::BindFrameBuffer(sm_SelectionBuffer);
		selectionBufferClearedThisFrame = true;

		Renderer::SetOrthographic();
		Bindables::Stencil::Resolve(Bindables::Stencil::Mode::NoDepth)->Bind();

		selectionPass2D.Execute();
	}
#endif // WITH_EDITOR

	//Reset for late rendering (imgui etc)
	Bindables::Stencil::Resolve(Bindables::Stencil::Mode::Off)->Bind();
	Renderer::SetPerspective();

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
