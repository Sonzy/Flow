#include "Flowpch.h"
#include "PlayerController.h"

namespace Flow
{
	PlayerController::PlayerController()
		: PlayerController("PlayerController")
	{
	}

	PlayerController::PlayerController(const std::string& Name)
		: Controller(Name), Camera(nullptr)
	{
	}

	void PlayerController::SetCamera(std::shared_ptr<CameraComponent> NewCamera)
	{
		CHECK_RETURN(!NewCamera, "PlayerController::SetCamera: Camera was nullptr");
		Camera = NewCamera;
	}
}