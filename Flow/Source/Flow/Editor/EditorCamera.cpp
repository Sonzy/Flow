#include "Flowpch.h"
#include "EditorCamera.h"
#include "Flow/Input/Input.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow/Events/KeyEvent.h"

EditorCamera::EditorCamera()
	: _Position(0), _Rotation(0), _MouseLastFrame(0), _CameraSpeed(0.2f)
{
}

DirectX::XMMATRIX EditorCamera::GetViewMatrix() const
{
	PROFILE_FUNCTION();

	const DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Rotator WorldRotation = Rotator::AsRadians(_Rotation);
	Vector WorldPosition = _Position;

	//Get Camera Look Vector
	const auto lookVector = DirectX::XMVector3Transform(Forward,
		DirectX::XMMatrixRotationRollPitchYaw(WorldRotation.Pitch, WorldRotation.Yaw, WorldRotation.Roll)
	);

	//Get the location the camera is looking
	DirectX::XMFLOAT3 fLookVector;
	DirectX::XMFLOAT3 camTarget;

	DirectX::XMStoreFloat3(&fLookVector, lookVector);
	camTarget.x = WorldPosition.X + fLookVector.x;
	camTarget.y = WorldPosition.Y + fLookVector.y;
	camTarget.z = WorldPosition.Z + fLookVector.z;

	//Return transformed Matrix with camera looking at a target
	DirectX::XMVECTOR vCamTarget = DirectX::XMLoadFloat3(&camTarget);
	DirectX::XMVECTOR camPosition = DirectX::XMLoadFloat3(&WorldPosition.ToDXFloat3());

	//Rotate the up vector
	return DirectX::XMMatrixLookAtLH(camPosition, vCamTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

void EditorCamera::MoveCamera(const Transform& NewTransform)
{
	_Position = NewTransform._Position;
	_Rotation = NewTransform._Rotation;
}

Transform EditorCamera::GetCameraTransform() const
{
	return Transform(_Position, _Rotation, Vector(1));
}

Vector EditorCamera::GetCameraPosition() const
{
	return _Position;
}

void EditorCamera::Update(float DeltaTime)
{
	if (!_CanUpdate)
		return;

	IntVector2D MousePosition = Input::GetMousePosition();
	Vector Translation(0.0f);

	if (Input::IsKeyPressed(FLOW_KEY_W))
		Translation.Z += _CameraSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_A))
		Translation.X += -_CameraSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_S))
		Translation.Z += -_CameraSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_D))
		Translation.X += _CameraSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_SPACE))
		Translation.Y += _CameraSpeed;
	if (Input::IsKeyPressed(FLOW_KEY_SHIFT))
		Translation.Y += -_CameraSpeed;

	//Camera rotation
	if (Input::IsMousePressed(FLOW_MOUSE_RIGHT))
	{
		if (_MouseLastFrame != MousePosition)
		{
			Vector Direction = (_MouseLastFrame - MousePosition);
			_Rotation.Yaw -= Direction.X * 0.25f; //Horizontal Sensitivity
			_Rotation.Pitch -= Direction.Y * 0.15f;  //Vertical Sensitivity
		}
	}

	_Position += _Rotation.RotateVector(Translation);

	_MouseLastFrame = MousePosition;

	CacheMatrices();
}