#pragma once

//= Includes ================================

#include <vector>
#include "Rendering/Core/RenderQueue/Job.h"

//= Enum Definitions =======================

enum class RenderPass
{
	Main = 0,
	OutlineMasking = 1,
	Outline = 2,
	NoDepth = 3,
	Standard2D = 4,
	UI = 5,
	Selection = 6,
	FrontFaceCulling = 7,
};

//= Class Definition ========================

class Pass
{
public:

	//= Public Functions ==========================

	Pass();
	
	void				Accept(Job NewJob);
	void				Execute() const;
	void				Reset();

	bool				IsEnabled() const;
	bool&				Enabled();
	void				SetEnabled(bool enabled);

private:

	//= Private Variables =========================

	std::vector<Job>	m_jobs;
	bool				m_enabled;
};