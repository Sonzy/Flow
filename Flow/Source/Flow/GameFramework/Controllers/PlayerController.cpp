#include "pch.h"
#include "PlayerController.h"

PlayerController::PlayerController()
	: PlayerController("PlayerController")
{
}

PlayerController::PlayerController(const std::string& Name)
	: Controller(Name), m_Camera(nullptr)
{
}

void PlayerController::SetCamera(CameraBase* NewCamera)
{
	CHECK_RETURN(!NewCamera, "PlayerController::SetCamera: Camera was nullptr");
	m_Camera = NewCamera;
}