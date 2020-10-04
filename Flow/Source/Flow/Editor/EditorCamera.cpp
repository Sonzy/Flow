#include "Flowpch.h"
#include "EditorCamera.h"
#include "Flow/Input/Input.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow/Events/KeyEvent.h"

EditorCamera::EditorCamera()
	: m_Position(0)
	, m_Rotation(0)
	, m_MouseLastFrame(0)
	, m_CameraSpeed(10.0f)
{
}

DirectX::XMMATRIX EditorCamera::GetViewMatrix() const
{
	PROFILE_FUNCTION();

	const DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Rotator WorldRotation = Rotator::AsRadians(m_Rotation);
	Vector3 WorldPosition = m_Position;

	//Get Camera Look Vector3
	const auto lookVector = DirectX::XMVector3Transform(Forward,
		DirectX::XMMatrixRotationRollPitchYaw(WorldRotation.Pitch, WorldRotation.Yaw, WorldRotation.Roll)
	);

	//Get the location the camera is looking
	DirectX::XMFLOAT3 fLookVector;
	DirectX::XMFLOAT3 camTarget;

	DirectX::XMStoreFloat3(&fLookVector, lookVector);
	camTarget.x = WorldPosition.x + fLookVector.x;
	camTarget.y = WorldPosition.y + fLookVector.y;
	camTarget.z = WorldPosition.z + fLookVector.z;

	//Return transformed Matrix with camera looking at a target
	DirectX::XMVECTOR vCamTarget = DirectX::XMLoadFloat3(&camTarget);
	DirectX::XMVECTOR camPosition = DirectX::XMLoadFloat3(&static_cast<DirectX::XMFLOAT3>(WorldPosition));

	//Rotate the up vector
	return DirectX::XMMatrixLookAtLH(camPosition, vCamTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

void EditorCamera::MoveCamera(const Transform& NewTransform)
{
	m_Position = NewTransform.m_Position;
	m_Rotation = NewTransform.m_Rotation;
}

Transform EditorCamera::GetCameraTransform() const
{
	return Transform(m_Position, m_Rotation, Vector3(1));
}

Vector3 EditorCamera::GetCameraPosition() const
{
	return m_Position;
}

void EditorCamera::Update(float DeltaTime)
{
	if (!m_CanUpdate)
		return;

	IntVector2 MousePosition = Input::GetMousePosition();
	Vector3 Translation(0.0f);

	//Camera rotation
	if (Input::IsMousePressed(MOUSE_RIGHT))
	{
		if (Input::IsKeyPressed(KEY_W))
			Translation.z += m_CameraSpeed;
		if (Input::IsKeyPressed(KEY_A))
			Translation.x += -m_CameraSpeed;
		if (Input::IsKeyPressed(KEY_S))
			Translation.z += -m_CameraSpeed;
		if (Input::IsKeyPressed(KEY_D))
			Translation.x += m_CameraSpeed;
		if (Input::IsKeyPressed(KEY_SPACE))
			Translation.y += m_CameraSpeed;
		if (Input::IsKeyPressed(KEY_SHIFT))

			Translation.y += -m_CameraSpeed;
		if (m_MouseLastFrame != MousePosition)
		{
			Vector3 Direction = m_MouseLastFrame - MousePosition;
			m_Rotation.Yaw -= Direction.x * 0.25f; //Horizontal Sensitivity
			m_Rotation.Pitch -= Direction.y * 0.15f;  //Vertical Sensitivity
		}
	}

	m_Position += m_Rotation.RotateVector(Translation * DeltaTime);

	m_MouseLastFrame = MousePosition;

	CacheMatrices();
}