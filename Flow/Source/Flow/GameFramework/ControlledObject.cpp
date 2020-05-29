#include "Flowpch.h"
#include "ControlledObject.h"
#include "Flow\Helper\HelperMacros.h"

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
	_Controller = NewController;

	OnControlled(_Controller);
}

void ControlledObject::RemoveControl()
{
	_Controller = nullptr;
}

void ControlledObject::OnControlled(Controller* OwningController)
{
}

Controller* ControlledObject::GetController() const
{
	return _Controller;
}