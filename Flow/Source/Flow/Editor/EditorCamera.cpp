#include "Flowpch.h"
#include "EditorCamera.h"
#include "Flow/Input/Input.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow/Events/KeyEvent.h"

EditorCamera::EditorCamera()
	: _Position(0), _Rotation(0), _MouseLastFrame(0), _CameraSpeed(10.0f)
{
}

DirectX::XMMATRIX EditorCamera::GetViewMatrix() const
{
	PROFILE_FUNCTION();

	const DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Rotator WorldRotation = Rotator::AsRadians(_Rotation);
	Vector3 WorldPosition = _Position;

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
	_Position = NewTransform.m_Position;
	_Rotation = NewTransform.m_Rotation;
}

Transform EditorCamera::GetCameraTransform() const
{
	return Transform(_Position, _Rotation, Vector3(1));
}

Vector3 EditorCamera::GetCameraPosition() const
{
	return _Position;
}

void EditorCamera::Update(float DeltaTime)
{
	if (!_CanUpdate)
		return;

	IntVector2 MousePosition = Input::GetMousePosition();
	Vector3 Translation(0.0f);



	//Camera rotation
	if (Input::IsMousePressed(FLOW_MOUSE_RIGHT))
	{
		if (Input::IsKeyPressed(FLOW_KEY_W))
			Translation.z += _CameraSpeed;
		if (Input::IsKeyPressed(FLOW_KEY_A))
			Translation.x += -_CameraSpeed;
		if (Input::IsKeyPressed(FLOW_KEY_S))
			Translation.z += -_CameraSpeed;
		if (Input::IsKeyPressed(FLOW_KEY_D))
			Translation.x += _CameraSpeed;
		if (Input::IsKeyPressed(FLOW_KEY_SPACE))
			Translation.y += _CameraSpeed;
		if (Input::IsKeyPressed(FLOW_KEY_SHIFT))

			Translation.y += -_CameraSpeed;
		if (_MouseLastFrame != MousePosition)
		{
			Vector3 Direction = _MouseLastFrame - MousePosition;
			_Rotation.Yaw -= Direction.x * 0.25f; //Horizontal Sensitivity
			_Rotation.Pitch -= Direction.y * 0.15f;  //Vertical Sensitivity
		}
	}

	_Position += _Rotation.RotateVector(Translation * DeltaTime);

	_MouseLastFrame = MousePosition;

	CacheMatrices();
}