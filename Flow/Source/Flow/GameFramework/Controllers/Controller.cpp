#include "Flowpch.h"
#include "Controller.h"
#include "Flow\GameFramework\Pawn.h"

Controller::Controller()
	: Controller("Controller")
{
}

Controller::Controller(const std::string& Name)
	: Actor(Name)
	, m_ControlledObject(nullptr)
	, m_Camera(nullptr)
{
}

void Controller::ControlObject(Pawn* Obj)
{
	if (m_ControlledObject)
	{
		RemoveControl(m_ControlledObject);
	}

	Obj->Control(this);
}

void Controller::RemoveControl(Pawn* Obj)
{
	CHECK_RETURN(!m_ControlledObject, "Controller::RemoveControl: Tried to remove control when no object is being controlled");

	m_ControlledObject->RemoveControl();
}

void Controller::SetCamera(std::shared_ptr<CameraBase> NewCamera)
{
	CHECK_RETURN(!NewCamera, "Controller::SetCamera: Tried to remove control when no object is being controlled");
	m_Camera = NewCamera;
}

std::shared_ptr<CameraBase> Controller::GetCamera() const
{
	return m_Camera;
}

bool Controller::IsLocalController() const
{
	return m_LocalController;
}

unsigned int Controller::GetControllerIndex() const
{
	return m_ControllerIndex;
}