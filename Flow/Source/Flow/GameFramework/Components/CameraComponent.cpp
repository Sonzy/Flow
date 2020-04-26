#include "Flowpch.h"
#include "CameraComponent.h"
#include "Flow\Input\Input.h"

namespace Flow
{
	CameraComponent::CameraComponent()
		: CameraComponent("Camera Component")
	{
	}

	CameraComponent::CameraComponent(const std::string& Name)
		: WorldComponent(Name), LastMousePosition_(0), MovementSpeed_(0.2f), RotationSpeed_(0.005f)
	{
		Projection_ = DirectX::XMMATRIX();
		FieldOfView_ = Math::DegreesToRadians(90.0f);
	}

	void CameraComponent::Tick(float DeltaTime)
	{
	}

	void CameraComponent::Update(float DeltaTime)
	{
		IntVector2D Pos = Input::GetMousePosition();

		//Camera rotation
		if (Input::IsMousePressed(FLOW_MOUSE_RIGHT))
		{
			if (LastMousePosition_ != Pos)
			{
				Vector Direction = (LastMousePosition_ - Pos);
				Direction.X = Direction.X * 0.25f; //Horizontal Sensitivity
				Direction.Y = Direction.Y * 0.15f;  //Vertical Sensitivity
				Rotator Test = GetWorldRotation();
				Rotator Change = Test.RotateRotator(Rotator(-Direction.Y, 0.0f, -Direction.X), false);
				SetWorldRotation(Change);
			}
		}

		LastMousePosition_ = Pos;

		CacheCameraMatrices();
	}


	DirectX::XMMATRIX CameraComponent::GetView() const
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
		//DirectX::XMFLOAT4 RotatedRead;
		//DirectX::XMStoreFloat4(&RotatedRead, Rotated);
		return DirectX::XMMatrixLookAtLH(camPosition, vCamTarget, Rotated);
	}

	Vector CameraComponent::GetCameraPosition() const
	{
		return GetWorldPosition();
	}
}