#pragma once

//= Includes ===================================

#include "Rendering/Core/RenderQueue/Job.h"
#include <vector>

//= Forward Declaration ========================

class Technique;
class Pass;

//= Class Definition ===========================

class FLOW_API RenderQueue
{
public:

	//= Public Static Functions ===================================

	static void				SubmitTechnique(Technique* Tech);
	static void				AcceptJob(Job NewJob, size_t TargetPass);
	static void				Execute();
	static RenderQueue*		Get();
	static void				Reset();
	static int				GetActiveRenderPass();

public:

	//= Public Functions =========================================

							RenderQueue();
							~RenderQueue();

public:

	//= Public Variables =========================================

	std::vector<Pass*>		m_Passes;

	bool					m_Pass0Enabled = true;
	bool					m_Pass1Enabled = true;
	bool					m_Pass2Enabled = true;
	bool					m_Pass3Enabled = true;
	bool					m_Pass4Enabled = true;
	bool					m_Pass5Enabled = true;
	bool					m_Pass6Enabled = true;
	bool					m_Pass7Enabled = true;

	int						m_CurrentPass;

private:

	//= Private Static Variables =================================

	static RenderQueue*		sm_RenderQueue;
};

