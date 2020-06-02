#include "Flowpch.h"
#include "Controller.h"
#include "Flow\GameFramework\Pawn.h"

Controller::Controller()
	: Controller("Controller")
{
}

Controller::Controller(const std::string& Name)
	: Actor(Name), _ControlledObject(nullptr), _Camera(nullptr)
{
}

void Controller::ControlObject(Pawn* Obj)
{
	if (_ControlledObject)
	{
		RemoveControl(_ControlledObject);
	}

	Obj->Control(this);
}

void Controller::RemoveControl(Pawn* Obj)
{
	CHECK_RETURN(!_ControlledObject, "Controller::RemoveControl: Tried to remove control when no object is being controlled");

	_ControlledObject->RemoveControl();
}

void Controller::SetCamera(std::shared_ptr<CameraBase> NewCamera)
{
	CHECK_RETURN(!NewCamera, "Controller::SetCamera: Tried to remove control when no object is being controlled");
	_Camera = NewCamera;
}

std::shared_ptr<CameraBase> Controller::GetCamera() const
{
	return _Camera;
}

bool Controller::IsLocalController() const
{
	return _LocalController;
}

unsigned int Controller::GetControllerIndex() const
{
	return _ControllerIndex;
}