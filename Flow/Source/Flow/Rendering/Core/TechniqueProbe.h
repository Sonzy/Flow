#pragma once

//= Forward Declarations ==================================

namespace DynamicCB { class Buffer; }
class Technique;
class Step;

//= Class Definitions =====================================

class TechniqueProbe
{
public:

	//= Public Functions ====================================================

	void				SetTechnique(Technique* Technique_In);
	void				SetStep(Step* Step_In);
	virtual bool		VisitBuffer(DynamicCB::Buffer&) = 0;

protected:

	//= Protected Functions ================================================

	virtual void		OnSetTechnique() {};
	virtual void		OnSetStep() {};

protected:

	//= Protected Variables ================================================

	Technique*			m_Technique;
	Step*				m_Step;
};