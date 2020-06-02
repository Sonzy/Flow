#pragma once
#include "Flow\GameFramework\Actor.h"

class Pawn;
class CameraBase;

/* Base class used for allowing anything to control an object */
class FLOW_API Controller : public Actor
{
public:
	Controller();
	Controller(const std::string& Name);

	virtual void ControlObject(Pawn* Obj);
	virtual void RemoveControl(Pawn* Obj);

	void SetCamera(std::shared_ptr<CameraBase> NewCamera);
	std::shared_ptr<CameraBase> GetCamera() const;

	bool IsLocalController() const;
	unsigned int GetControllerIndex() const;

protected:
	Pawn* _ControlledObject;
	std::shared_ptr<CameraBase> _Camera;

	bool _LocalController = true;
	unsigned int _ControllerIndex = 0;
};