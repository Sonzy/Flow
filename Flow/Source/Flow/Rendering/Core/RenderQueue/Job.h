#pragma once

//= Class Definitions =============================

class Step;
class Renderable;

//= Class Definition =============================

class Job
{
public:

	//= Public Functions ================================

						Job(const Step* NewStep, const Renderable* Drawable);
	void				Execute() const;

private:

	//= Private Variables ===============================

	const Step*			m_Step;
	const Renderable*	m_Renderable;
};