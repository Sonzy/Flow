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

	bool _Pass0Enabled = true;
	bool _Pass1Enabled = true;
	bool _Pass2Enabled = true;
	bool _Pass3Enabled = true;
	bool _Pass4Enabled = true;

private:

	static RenderQueue* s_RenderQueue;
};

