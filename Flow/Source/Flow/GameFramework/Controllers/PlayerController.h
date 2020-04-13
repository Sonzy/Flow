#pragma once
#include "Controller.h"

namespace Flow
{
	class CameraComponent;
	class WorldObject;

	class PlayerController : public Controller
	{
	public:
		PlayerController();
		PlayerController(const std::string& Name);

		void SetCamera(std::shared_ptr<CameraComponent> NewCamera);

	protected:

		std::shared_ptr<CameraComponent> Camera;
		std::shared_ptr<WorldObject> Pawn;
	};
}
