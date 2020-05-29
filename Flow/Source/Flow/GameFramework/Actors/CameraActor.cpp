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
		Translation.Z = 1.0f;
	if (Input::IsKeyPressed(FLOW_KEY_A))
		Translation.X = -1.0f;
	if (Input::IsKeyPressed(FLOW_KEY_S))
		Translation.Z = -1.0f;
	if (Input::IsKeyPressed(FLOW_KEY_D))
		Translation.X = 1.0f;
	if (Input::IsKeyPressed(FLOW_KEY_SPACE))
		Translation.Y = 1.0f;
	if (Input::IsKeyPressed(FLOW_KEY_SHIFT))
		Translation.Y = -1.0f;

	//Camera rotation
	if (Input::IsMousePressed(FLOW_MOUSE_RIGHT))
	{
		if (_LastMousePosition != Pos)
		{
			Vector Direction = (_LastMousePosition - Pos);
			Direction.X = Direction.X * 0.25f; //Horizontal Sensitivity
			Direction.Y = Direction.Y * 0.15f;  //Vertical Sensitivity
			_RootComponent->AddRelativeRotation(Rotator(-Direction.Y, 0.0f, -Direction.X));
		}
	}

	_RootComponent->SetRelativePosition(_RootComponent->GetRelativePosition() + _RootComponent->GetRelativeRotation().RotateVector(Translation));
	_LastMousePosition = Pos;

	//TODO:
	//CacheCameraMatrices();
}

void CameraActor::SetMainCamera() const
{
	RenderCommand::SetMainCamera(_CameraComponent);
}
