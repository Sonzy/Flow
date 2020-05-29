#include "Flowpch.h"
#include "PlayerController.h"

PlayerController::PlayerController()
	: PlayerController("PlayerController")
{
}

PlayerController::PlayerController(const std::string& Name)
	: Controller(Name), _Camera(nullptr)
{
}

void PlayerController::SetCamera(std::shared_ptr<CameraBase> NewCamera)
{
	CHECK_RETURN(!NewCamera, "PlayerController::SetCamera: Camera was nullptr");
	_Camera = NewCamera;
}