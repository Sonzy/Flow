#include "pch.h"

#if WITH_EDITOR

#include "EditorCamera.h"
#include "Framework/Input/Input.h"
#include "Framework/Events/MouseEvent.h"
#include "Framework/Events/KeyEvent.h"
#include "Editor/Editor.h"

#include "Rendering/Renderer.h"

EditorCamera::EditorCamera()
	: m_Position(0)
	, m_Rotation(0)
	, m_MouseLastFrame(0)
	, m_mousePanning(false)
{
}

DirectX::XMMATRIX EditorCamera::GetViewMatrix() const
{
	PROFILE_FUNCTION();

	const DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Rotator WorldRotation = Rotator::AsRadians(m_Rotation);
	const DirectX::XMFLOAT3 WorldPosition = m_Position;

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
	DirectX::XMVECTOR camPosition = DirectX::XMLoadFloat3(&WorldPosition);

	//Rotate the up vector
	return DirectX::XMMatrixLookAtLH(camPosition, vCamTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

DirectX::XMMATRIX EditorCamera::GetViewMatrix2D() const
{
	return DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	
	//=================================

	//IntVector2 size = Renderer::GetWindowSize();
	//float xVal = (float)size.x / 2.0f;
	//float yVal = (float)size.y / 2.0f;
	//
	//DirectX::XMFLOAT4X4 viewMatrix =
	//{
	//	1,      0,      0,      0,
	//	0,      1,      0,      0,
	//	0,      0,      1,      0,
	//	-xVal, -yVal,   0,      1
	//};
	//
	//return DirectX::XMLoadFloat4x4(&viewMatrix);

	//=================================

	//const DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	//Rotator WorldRotation = Rotator(0.0f, 0.0f, 0.0f);
	////const DirectX::XMFLOAT3 WorldPosition = Vector3(0.0f, 0.0f, 0.0f);
	//const DirectX::XMFLOAT3 WorldPosition = m_Position;
	//
	////Get Camera Look Vector3
	//const auto lookVector = DirectX::XMVector3Transform(Forward,
	//	DirectX::XMMatrixRotationRollPitchYaw(WorldRotation.Pitch, WorldRotation.Yaw, WorldRotation.Roll)
	//);
	//
	////Get the location the camera is looking
	//DirectX::XMFLOAT3 fLookVector;
	//DirectX::XMFLOAT3 camTarget;
	//
	//DirectX::XMStoreFloat3(&fLookVector, lookVector);
	//camTarget.x = WorldPosition.x + fLookVector.x;
	//camTarget.y = WorldPosition.y + fLookVector.y;
	//camTarget.z = WorldPosition.z + fLookVector.z;
	//
	////Return transformed Matrix with camera looking at a target
	//DirectX::XMVECTOR vCamTarget = DirectX::XMLoadFloat3(&camTarget);
	//DirectX::XMVECTOR camPosition = DirectX::XMLoadFloat3(&WorldPosition);
	//
	//DirectX::XMMATRIX lookat = DirectX::XMMatrixLookAtLH(camPosition, vCamTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));	//Rotate the up vector
	//return lookat;
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

bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e)
{
	Editor::Settings& settings = Editor::GetSettings();

	Vector3 translation = Vector3(0.0f, 0.0f, e.GetYOffset());
	m_Position += m_Rotation.RotateVector(translation * settings.m_cameraScrollingSpeed);
	return false;
}

void EditorCamera::Update(float DeltaTime)
{
	if (!m_CanUpdate)
		return;

	Editor::Settings& settings = Editor::GetSettings();

	IntVector2 MousePosition = Input::GetMousePosition();
	Vector3 Translation(0.0f);

	if (Input::IsMousePressed(MOUSE_MIDDLE))
	{
		Vector3 offset = Vector3(m_MouseLastFrame.x - MousePosition.x, MousePosition.y - m_MouseLastFrame.y, 0);
		m_Position += m_Rotation.RotateVector(offset * DeltaTime * settings.m_cameraPanningSpeed);
		m_mousePanning = true;
	}
	else
	{
		m_mousePanning = false;
	}

	//Camera rotation
	if (m_mousePanning == false && Input::IsMousePressed(MOUSE_RIGHT))
	{
		if (Input::IsKeyPressed(KEY_W))
			Translation.z += settings.m_cameraSpeed;
		if (Input::IsKeyPressed(KEY_A))
			Translation.x += -settings.m_cameraSpeed;
		if (Input::IsKeyPressed(KEY_S))
			Translation.z += -settings.m_cameraSpeed;
		if (Input::IsKeyPressed(KEY_D))
			Translation.x += settings.m_cameraSpeed;
		if (Input::IsKeyPressed(KEY_SPACE))
			Translation.y += settings.m_cameraSpeed;
		if (Input::IsKeyPressed(KEY_SHIFT))

			Translation.y += -settings.m_cameraSpeed;
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

#endif //WITH_EDITOR