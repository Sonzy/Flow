#pragma once
#include "Flow/Rendering/Core/RenderQueue/Job.h"
#include <vector>

class Technique;
class Pass;

class FLOW_API RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();
	//= Jobs ==========

	static void SubmitTechnique(Technique* Tech);
	static void AcceptJob(Job NewJob, size_t TargetPass);
	static void Execute();
	static RenderQueue* Get();
	static void Reset();

	std::vector<Pass*> _Passes;

	bool m_Pass0Enabled = true;
	bool m_Pass1Enabled = true;
	bool m_Pass2Enabled = true;
	bool m_Pass3Enabled = true;
	bool m_Pass4Enabled = true;
	bool m_Pass5Enabled = true;

private:

	static RenderQueue* s_RenderQueue;
};

