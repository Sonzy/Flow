#pragma once

class Bindable;
class Renderable;
class TechniqueProbe;

class Step
{
public:
	Step(size_t TargetPass);

	void Submit(const Renderable& Drawable) const;
	void Bind() const;
	void InitialiseParentReferences(const Renderable& Drawable);
	void AddBindable(std::shared_ptr<Bindable> NewBind);
	void AddBindables(std::vector<std::shared_ptr<Bindable>> Binds);

	void AcceptProbe(TechniqueProbe& Probe);

private:

	size_t _TargetPass;
	std::vector<std::shared_ptr<Bindable>> _Bindables;
};