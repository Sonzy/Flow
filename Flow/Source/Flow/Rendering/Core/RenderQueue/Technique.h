#pragma once

//= Includes ============================

#include <vector>
#include "Step.h"

//= Forward Declarations ================

class Renderable;
class TechniqueProbe;

//= Class Definition ====================

class Technique
{
public:

	//= Public Functions ====================================

						Technique(const std::string& Name);

	void				Submit(const Renderable& Drawable) const;
	void				AddStep(Step NewStep);

	void				Activate();
	void				Deactivate();
	void				SetActive(bool NewActive) { m_Active = NewActive; }
	bool				IsActive() const { return m_Active; }
	bool&				GetWriteAccessToActive() { return m_Active; }

	void				InitialiseParentReferences(const Renderable& Parent);

	void				AcceptProbe(TechniqueProbe& Probe);

	const std::string&	GetName() const { return m_Name; }

private:

	//= Private Variables ====================================

	bool				m_Active = true;
	std::vector<Step>	m_Steps;
	std::string			m_Name;
};