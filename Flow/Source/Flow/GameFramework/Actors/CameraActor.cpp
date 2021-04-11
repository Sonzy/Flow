#include "pch.h"
#include "CameraActor.h"
#include "Flow/Rendering/Renderer.h"
#include "Flow/GameFramework/Components/CameraComponent.h"
#include "Flow/GameFramework/Controllers/Controller.h"
#include "Framework/Input/Input.h"

CameraActor::CameraActor()
	: CameraActor("Camera Actor")
{
}

CameraActor::CameraActor(const std::string& Name)
	: Pawn(Name), m_CameraMovementSpeed(1.0f), m_CameraHorizontalRotationSpeed(0.25f), m_CameraVerticalRotationSpeed(0.15f)
{
	m_CameraComponent = CreateComponent<CameraComponent>("Camera");
	m_CameraComponent->SetOwned(false);
	m_RootComponent = m_CameraComponent;
}

void CameraActor::OnControlled(Controller* OwningController)
{
	SetMainCamera();
	OwningController->SetCamera(m_CameraComponent);
}

void CameraActor::Tick(float DeltaTime)
{
	PROFILE_FUNCTION();

	IntVector2 Pos = Input::GetMousePosition();

	Vector3 Translation(0.0f);

	if (Input::IsKeyPressed(KEY_W))
		Translation.z = m_CameraMovementSpeed;
	if (Input::IsKeyPressed(KEY_A))
		Translation.x = -m_CameraMovementSpeed;
	if (Input::IsKeyPressed(KEY_S))
		Translation.z = -m_CameraMovementSpeed;
	if (Input::IsKeyPressed(KEY_D))
		Translation.x = m_CameraMovementSpeed;
	if (Input::IsKeyPressed(KEY_SPACE))
		Translation.y = m_CameraMovementSpeed;
	if (Input::IsKeyPressed(KEY_SHIFT))
		Translation.y = -m_CameraMovementSpeed;

	//Camera rotation
	if (Input::IsMousePressed(MOUSE_RIGHT))
	{
		if (m_LastMousePosition != Pos)
		{
			Vector3 Direction = (m_LastMousePosition - Pos);
			Direction.x = Direction.x * m_CameraHorizontalRotationSpeed; //Horizontal Sensitivity
			Direction.y = Direction.y * m_CameraVerticalRotationSpeed;  //Vertical Sensitivity
			m_RootComponent->AddRelativeRotation(Rotator(-Direction.y, 0.0f, -Direction.x));
		}
	}

	m_RootComponent->SetRelativePosition(m_RootComponent->GetRelativePosition() + m_RootComponent->GetRelativeRotation().RotateVector(Translation));
	m_LastMousePosition = Pos;

	m_CameraComponent->CacheMatrices();
}

void CameraActor::SetMainCamera() const
{
	Renderer::SetMainCamera(m_CameraComponent);
}