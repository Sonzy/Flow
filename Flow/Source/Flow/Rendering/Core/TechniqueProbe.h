#pragma once

namespace DCB
{
	class Buffer;
}

class Technique;
class Step;

class TechniqueProbe
{
public:

	void SetTechnique(Technique* Technique_In);
	void SetStep(Step* Step_In);
	virtual bool VisitBuffer(DCB::Buffer&) = 0;

protected:

	virtual void OnSetTechnique() {};
	virtual void OnSetStep() {};

protected:

	Technique* _Technique;
	Step* _Step;
};