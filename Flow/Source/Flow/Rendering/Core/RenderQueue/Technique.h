#pragma once
#include "Step.h"
#include <vector>

class Renderable;
class TechniqueProbe;

class Technique
{
public:
	Technique(const std::string& Name);

	void Submit(const Renderable& Drawable) const;
	void AddStep(Step NewStep);

	void Activate();
	void Deactivate();
	void SetActive(bool NewActive) { _Active = NewActive; }
	bool IsActive() const { return _Active; }
	bool& GetWriteAccessToActive() { return _Active; }

	void InitialiseParentReferences(const Renderable& Parent);

	void AcceptProbe(TechniqueProbe& Probe);

	const std::string& GetName() const { return m_Name; }

private:

	bool _Active = true;
	std::vector<Step> _Steps;
	std::string m_Name;
};