#pragma once
#include "Step.h"
#include <vector>

class Renderable;

class Technique
{
public:
	void Submit(const Renderable& Drawable) const;
	void AddStep(Step NewStep);

	void Activate();
	void Deactivate();
	bool IsActive() { return _Active; }
	bool& GetWriteAccessToActive() { return _Active; }

	void InitialiseParentReferences(const Renderable& Parent);

private:

	bool _Active = true;
	std::vector<Step> _Steps;
};