#include "Flowpch.h"
#include "CameraActor.h"
#include "Flow/Rendering/RenderCommand.h"
#include "Flow/GameFramework/Components/CameraComponent.h"
#include "Flow/GameFramework/Controllers/Controller.h"
#include "Flow/Input/Input.h"

CameraActor::CameraActor()
	: CameraActor("Camera Actor")
{
}

CameraActor::CameraActor(const std::string& Name)
	: Pawn(Name), CameraMovementSpeed(1.0f), CameraHorizontalRotationSpeed(0.25f), CameraVerticalRotationSpeed(0.15f)
{
	_CameraComponent = CreateComponent<CameraComponent>("Camera");

	_RootComponent = _CameraComponent.get();
}

void CameraActor::OnControlled(Controller* OwningController)
{
	SetMainCamera();
	OwningController->SetCamera(_CameraComponent);
}

void CameraActor::Tick(float DeltaTime)
{
	PROFILE_FUNCTION();

	IntVector2 Pos = Input::GetMousePosition();

	Vector3 Translation(0.0f);

	if (Input::IsKeyPressed(FLOW_KEY_W))
		Translation.z = CameraMovementSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_A))
		Translation.x = -CameraMovementSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_S))
		Translation.z = -CameraMovementSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_D))
		Translation.x = CameraMovementSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_SPACE))
		Translation.y = CameraMovementSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_SHIFT))
		Translation.y = -CameraMovementSpeed;

	//Camera rotation
	if (Input::IsMousePressed(FLOW_MOUSE_RIGHT))
	{
		if (_LastMousePosition != Pos)
		{
			Vector3 Direction = (_LastMousePosition - Pos);
			Direction.x = Direction.x * CameraHorizontalRotationSpeed; //Horizontal Sensitivity
			Direction.y = Direction.y * CameraVerticalRotationSpeed;  //Vertical Sensitivity
			_RootComponent->AddRelativeRotation(Rotator(-Direction.y, 0.0f, -Direction.x));
		}
	}

	_RootComponent->SetRelativePosition(_RootComponent->GetRelativePosition() + _RootComponent->GetRelativeRotation().RotateVector(Translation));
	_LastMousePosition = Pos;

	_CameraComponent->CacheMatrices();
}

void CameraActor::SetMainCamera() const
{
	RenderCommand::SetMainCamera(_CameraComponent);
}
