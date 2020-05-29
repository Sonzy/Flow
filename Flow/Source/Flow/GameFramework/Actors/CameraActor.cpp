#include "Flowpch.h"
#include "CameraActor.h"
#include "Flow/Rendering/RenderCommand.h"
#include "Flow/GameFramework/Components/CameraComponent.h"
#include "Flow/GameFramework/Controllers/Controller.h"
#include "Flow/Input/Input.h"
#include "Flow/Helper/Maths/Transformations/Rotator.h"

CameraActor::CameraActor()
	: CameraActor("Camera Actor")
{
}

CameraActor::CameraActor(const std::string& Name)
	: ControlledObject(Name), CameraMovementSpeed(1.0f), CameraHorizontalRotationSpeed(0.25f), CameraVerticalRotationSpeed(0.15f)
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
	IntVector2D Pos = Input::GetMousePosition();

	Vector Translation(0.0f);

	if (Input::IsKeyPressed(FLOW_KEY_W))
		Translation.Z = CameraMovementSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_A))
		Translation.X = -CameraMovementSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_S))
		Translation.Z = -CameraMovementSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_D))
		Translation.X = CameraMovementSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_SPACE))
		Translation.Y = CameraMovementSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_SHIFT))
		Translation.Y = -CameraMovementSpeed;

	//Camera rotation
	if (Input::IsMousePressed(FLOW_MOUSE_RIGHT))
	{
		if (_LastMousePosition != Pos)
		{
			Vector Direction = (_LastMousePosition - Pos);
			Direction.X = Direction.X * CameraHorizontalRotationSpeed; //Horizontal Sensitivity
			Direction.Y = Direction.Y * CameraVerticalRotationSpeed;  //Vertical Sensitivity
			_RootComponent->AddRelativeRotation(Rotator(-Direction.Y, 0.0f, -Direction.X));
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
