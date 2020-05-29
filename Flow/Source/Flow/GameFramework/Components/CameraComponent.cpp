#include "Flowpch.h"
#include "CameraComponent.h"
#include "Flow\Input\Input.h"

CameraComponent::CameraComponent()
	: CameraComponent("Camera Component")
{
}

CameraComponent::CameraComponent(const std::string& Name)
	: WorldComponent(Name), _LastMousePosition(0), _MovementSpeed(0.2f), _RotationSpeed(0.005f)
{
	_Projection = DirectX::XMMATRIX();
	_FieldOfView = Math::DegreesToRadians(90.0f);
}

void CameraComponent::Update(float DeltaTime)
{
	IntVector2D Pos = Flow::Input::GetMousePosition();

	//Camera rotation
	if (Flow::Input::IsMousePressed(FLOW_MOUSE_RIGHT))
	{
		if (_LastMousePosition != Pos)
		{
			Vector Direction = (_LastMousePosition - Pos);
			Direction.X = Direction.X * 0.25f; //Horizontal Sensitivity
			Direction.Y = Direction.Y * 0.15f;  //Vertical Sensitivity
			Rotator Test = GetWorldRotation();
			Rotator Change = Test.RotateRotator(Rotator(-Direction.Y, 0.0f, -Direction.X), false);
			SetWorldRotation(Change);
		}
	}

	_LastMousePosition = Pos;

	CacheMatrices();
}


DirectX::XMMATRIX CameraComponent::GetViewMatrix() const
{
	const DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Rotator WorldRotation = Rotator::AsRadians(GetWorldRotation());
	Vector WorldPosition = GetWorldPosition();

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
	DirectX::XMVECTOR Position = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR Rotation = DirectX::XMQuaternionRotationRollPitchYaw(WorldRotation.Pitch, WorldRotation.Yaw, WorldRotation.Roll);

	DirectX::XMVECTOR Rotated = DirectX::XMVector3Rotate(Position, Rotation);
	return DirectX::XMMatrixLookAtLH(camPosition, vCamTarget, Rotated);
}

Vector CameraComponent::GetCameraPosition() const
{
	return GetWorldPosition();
}