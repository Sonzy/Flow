#pragma once
#include "Controller.h"

class CameraBase;
class WorldObject;

class PlayerController : public Controller
{
public:
	PlayerController();
	PlayerController(const std::string& Name);

	void SetCamera(std::shared_ptr<CameraBase> NewCamera);

protected:

	std::shared_ptr<CameraBase> _Camera;
	std::shared_ptr<WorldObject> _Pawn;
};
