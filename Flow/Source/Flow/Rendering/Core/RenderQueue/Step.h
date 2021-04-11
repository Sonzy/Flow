#pragma once

// Includes ////////////////////////////////////

#include "Rendering/Core/RenderQueue/Pass.h"

//= Forward Declarations =======================

namespace Bindables
{
	class Bindable;
}

class Renderable;
class TechniqueProbe;

//= Class Definition ===========================

class Step
{
public:

	//= Public Functions ========================================

						Step(RenderPass TargetPass);

	void				Submit(const Renderable& Drawable) const;
	void				Bind() const;
	void				InitialiseParentReferences(const Renderable& Drawable);
	void				AddBindable(Bindables::Bindable* NewBind);
	void				AddBindables(std::vector<Bindables::Bindable*> Binds);

	void				AcceptProbe(TechniqueProbe& Probe);

private:

	//= Private Variables ======================================

	RenderPass							m_TargetPass;
	std::vector<Bindables::Bindable*>	m_Bindables;
};