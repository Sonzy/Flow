#include "Flowpch.h"
#include "CameraComponent.h"
#include "Flow\Input\Input.h"

CameraComponent::CameraComponent()
	: CameraComponent("Camera Component")
{
}

CameraComponent::CameraComponent(const std::string& Name)
	: WorldComponent(Name)
{
	_Projection = DirectX::XMMATRIX();
	_FieldOfView = Math::DegreesToRadians(90.0f);
}

void CameraComponent::Update(float DeltaTime)
{

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