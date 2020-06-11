#pragma once

class Step;
class Renderable;

class Job
{
public:

	Job(const Step* NewStep, const Renderable* Drawable);

	void Execute() const;

private:

	const Step* _Step;
	const Renderable* _Renderable;
};