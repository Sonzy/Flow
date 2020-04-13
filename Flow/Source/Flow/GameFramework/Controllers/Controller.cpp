#include "Flowpch.h"
#include "Controller.h"
#include "Flow\GameFramework\ControlledObject.h"

namespace Flow
{
	Controller::Controller()
		: Controller("Controller")
	{
	}

	Controller::Controller(const std::string& Name)
		: WorldObject(Name), ControlledObject_(nullptr), Camera_(nullptr)
	{
	}

	void Controller::ControlObject(ControlledObject* Obj)
	{
		if (ControlledObject_)
		{
			RemoveControl(ControlledObject_);
		}

		Obj->Control(this);
	}

	void Controller::RemoveControl(ControlledObject* Obj)
	{
		CHECK_RETURN(!ControlledObject_, "Controller::RemoveControl: Tried to remove control when no object is being controlled");

		ControlledObject_->RemoveControl();
	}

	void Controller::SetCamera(CameraComponent* NewCamera)
	{
		CHECK_RETURN(!NewCamera, "Controller::SetCamera: Tried to remove control when no object is being controlled");
		Camera_ = NewCamera;
	}

	CameraComponent* Controller::GetCamera() const
	{
		return Camera_;
	}

	bool Controller::IsLocalController() const
	{
		return LocalController_;
	}

	unsigned int Controller::GetControllerIndex() const
	{
		return ControllerIndex_;
	}
}