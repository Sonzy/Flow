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
		: WorldComponent(Name), LastMousePosition_(0), Projection_(DirectX::XMMATRIX()), FOV_(Math::DegreesToRadians(90.0f)),
		MovementSpeed_(0.2f), RotationSpeed_(0.005f)
	{
	}

	void CameraComponent::Tick(float DeltaTime)
	{
		return;

		IntVector2D Pos = Input::GetMousePosition();

		Vector Translation(0.0f);

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

		AddRelativePosition(Translation);

		LastMousePosition_ = Pos;
	}

	void CameraComponent::SetFOV(float FOV)
	{
		FOV_ = FOV;
	}

	void CameraComponent::SetProjection(DirectX::XMMATRIX Projection)
	{
		Projection_ = Projection;
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

		return DirectX::XMMatrixLookAtLH(camPosition, vCamTarget, DirectX::XMVector3Rotate(Position, Rotation));
	}

	DirectX::XMMATRIX CameraComponent::GetProjectionMatrix() const
	{
		return Projection_;
	}

	float CameraComponent::GetFOV() const
	{
		return FOV_;
	}
}