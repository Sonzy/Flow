#pragma once

//= Includes ================================

#include <vector>
#include "Rendering/Core/RenderQueue/Job.h"

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