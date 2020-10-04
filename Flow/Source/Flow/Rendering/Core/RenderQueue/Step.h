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
	void				AddBindable(std::shared_ptr<Bindable> NewBind);
	void				AddBindables(std::vector<std::shared_ptr<Bindable>> Binds);

	void				AcceptProbe(TechniqueProbe& Probe);

private:

	//= Private Variables ======================================

	size_t									m_TargetPass;
	std::vector<std::shared_ptr<Bindable>>	m_Bindables;
};