#pragma once

// Includes ////////////////////////////////////////////

#include <vector>
#include "Rendering/Core/RenderQueue/Job.h"
#include "Rendering/Core/RenderQueue/Pass.h"

#include "Rendering/Other/FrameBuffer.h"

// Forward Declaration /////////////////////////////////

class Technique;

// Class Definition ////////////////////////////////////

class FLOW_API RenderQueue
{
public:

	// Public Static Functions ////////////////////////////////////

	static void				SubmitTechnique(Technique* Tech);
	static void				AcceptJob(Job NewJob, RenderPass TargetPass);
	static void				Execute();
	static void				Reset();
	static RenderPass		GetActiveRenderPass();
	static Pass&			GetPass(RenderPass pass);
	static const FrameBuffer* GetSelectionBuffer() { return sm_SelectionBuffer; }

public:

	// Public Functions ////////////////////////////////////////////

							RenderQueue();
							~RenderQueue();

public:

	// Public Static Variables ////////////////////////////////////////////

	static std::unordered_map<RenderPass, Pass>		sm_Passes;
	static RenderPass								sm_CurrentPass;
	static FrameBuffer*								sm_SelectionBuffer;
};

