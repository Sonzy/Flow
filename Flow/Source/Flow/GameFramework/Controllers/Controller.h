#pragma once
#include "Flow\GameFramework\WorldObject.h"

class ControlledObject;
class CameraBase;

/* Base class used for allowing anything to control an object */
class FLOW_API Controller : public WorldObject
{
public:
	Controller();
	Controller(const std::string& Name);

	virtual void ControlObject(ControlledObject* Obj);
	virtual void RemoveControl(ControlledObject* Obj);

	void SetCamera(std::shared_ptr<CameraBase> NewCamera);
	std::shared_ptr<CameraBase> GetCamera() const;

	bool IsLocalController() const;
	unsigned int GetControllerIndex() const;

protected:
	ControlledObject* _ControlledObject;
	std::shared_ptr<CameraBase> _Camera;

	bool _LocalController = true;
	unsigned int _ControllerIndex = 0;
};