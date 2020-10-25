#pragma once

//= Forward Declarations =======================

class Bindable;
class Renderable;
class TechniqueProbe;

//= Class Definition ===========================

class Step
{
public:

	//= Public Functions ========================================

						Step(size_t TargetPass);

	void				Submit(const Renderable& Drawable) const;
	void				Bind() const;
	void				InitialiseParentReferences(const Renderable& Drawable);
	void				AddBindable(Bindable* NewBind);
	void				AddBindables(std::vector<Bindable*> Binds);

	void				AcceptProbe(TechniqueProbe& Probe);

private:

	//= Private Variables ======================================

	size_t									m_TargetPass;
	std::vector<Bindable*>	m_Bindables;
};