#include "Flowpch.h"
#include "ControlledObject.h"
#include "Flow\Helper\HelperMacros.h"

namespace Flow
{
	ControlledObject::ControlledObject()
		: ControlledObject("ControlledObject")
	{
	}

	ControlledObject::ControlledObject(const std::string& Name)
		: WorldObject(Name)
	{
	}

	void ControlledObject::Control(Controller* NewController)
	{
		CHECK_RETURN(!NewController, "ControlledObject::Control: Controller was nullptr");
		Controller_ = NewController;
	}

	void ControlledObject::RemoveControl()
	{
		Controller_ = nullptr;
	}

	Controller* ControlledObject::GetController() const
	{
		return Controller_;
	}
}