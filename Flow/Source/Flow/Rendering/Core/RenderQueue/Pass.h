#pragma once

//= Includes ================================

#include <vector>
#include "Rendering/Core/RenderQueue/Job.h"

//= Macro Definitions =======================

//TODO: manage these better
#define PASS_MAIN 0
#define PASS_FRONT_CULLED 1
#define PASS_NO_CULLING 2
#define PASS_OUTLINE_MASKING 3
#define PASS_OUTLINE 4
#define PASS_NO_DEPTH 5
#define PASS_2D 6
#define PASS_UI 7

//= Class Definition ========================

class Pass
{
public:

	//= Public Functions ==========================

	void				Accept(Job NewJob);
	void				Execute() const;
	void				Reset();

private:

	//= Private Variables =========================

	std::vector<Job>	m_Jobs;
};