#pragma once
#include "Flow\GameFramework\WorldObject.h"

class Controller;

class FLOW_API ControlledObject : public WorldObject
{
public:
	ControlledObject();
	ControlledObject(const std::string& Name);

	void Control(Controller* NewController);
	void RemoveControl();

	virtual void OnControlled(Controller* OwningController);

	Controller* GetController() const;

private:

	Controller* _Controller;
};