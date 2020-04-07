#include "Flowpch.h"
#include "Camera.h"
#include "Flow/Helper/Maths.h"
#include "ThirdParty/ImGui/imgui.h"
#include <algorithm>
#include "Flow\Input\Input.h"

namespace Flow
{
	Camera::Camera()
		: Position_(0.0f,0.0f, -50.0f), Rotation_(0.0f), LastMousePosition_(Input::GetMousePosition())
	{
	}

	DirectX::XMMATRIX Camera::GetMatrix() const
	{
		const DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		
		//Get Camera Look Vector
		const auto lookVector = DirectX::XMVector3Transform(Forward,
			DirectX::XMMatrixRotationRollPitchYaw(Math::DegreesToRadians(Rotation_.Pitch), Math::DegreesToRadians(Rotation_.Yaw), Math::DegreesToRadians(Rotation_.Roll))
		);
		
		//Get the location the camera is looking
		DirectX::XMFLOAT3 fLookVector;
		DirectX::XMFLOAT3 camTarget;
		
		DirectX::XMStoreFloat3(&fLookVector, lookVector);
		camTarget.x = Position_.X + fLookVector.x;
		camTarget.y = Position_.Y + fLookVector.y;
		camTarget.z = Position_.Z + fLookVector.z;
		
		//Return transformed Matrix with camera looking at a target
		DirectX::XMVECTOR vCamTarget = DirectX::XMLoadFloat3(&camTarget);
		DirectX::XMVECTOR camPosition = DirectX::XMLoadFloat3(&Position_.ToDXFloat3());
		return DirectX::XMMatrixLookAtLH(camPosition, vCamTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	}

	void Camera::Reset()
	{
		Position_ = { 0.0f, 0.0f, -10.0f };

		Rotation_.Pitch = 0.0f;
		Rotation_.Roll = 0.0f;
	}

	void Camera::Translate(DirectX::XMFLOAT3 Translation)
	{
		DirectX::XMStoreFloat3(&Translation, DirectX::XMVector3Transform(
			DirectX::XMLoadFloat3(&Translation),
			DirectX::XMMatrixRotationRollPitchYaw(Math::DegreesToRadians(Rotation_.Pitch), Math::DegreesToRadians(Rotation_.Yaw), Math::DegreesToRadians(Rotation_.Roll)) *
			DirectX::XMMatrixScaling(CameraSpeed_, CameraSpeed_, CameraSpeed_)
		));

		Position_.X += Translation.x;
		Position_.Y += Translation.y;
		Position_.Z += Translation.z;
	}

	void Camera::SetRotation(Rotator Rotation)
	{
		Rotation_ = Rotation;
	}

	void Camera::Rotate(Rotator Offset)
	{
		Rotation_ += Offset;
	}

	void Camera::Tick(float DeltaTime)
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
			if (LastMousePosition_ != Pos)
			{
				Vector Direction = (LastMousePosition_ - Pos);
				Direction.X = Direction.X * 0.25f; //Horizontal Sensitivity
				Direction.Y = Direction.Y * 0.15f;  //Vertical Sensitivity
				Rotate(Rotator(-Direction.Y, 0.0f, -Direction.X));
			}
		}

		Translate(Translation.ToDXFloat3());

		LastMousePosition_ = Pos;
	}

	void Camera::RenderIMGUIWindow()
	{
		if (ImGui::Begin("Camera"))
		{
			ImGui::Text("Position");
			ImGui::SliderFloat("X", &Position_.X, -200.0f, 200.0f, "%.1f");
			ImGui::SliderFloat("Y", &Position_.Y, -200.0f, 200.0f, "%.1f");
			ImGui::SliderFloat("Z", &Position_.Z, -200.0f, 200.0f, "%.1f");
			ImGui::Text("Rotation");
			ImGui::Text("Pitch: %f", Rotation_.Pitch);
			ImGui::Text("Roll: %f", Rotation_.Roll);
			ImGui::Text("Yaw: %f", Rotation_.Yaw);
			ImGui::Text("Other");
			ImGui::SliderAngle("Field Of View", &m_FOV, 60.0f, 100.0f);

			if (ImGui::Button("Reset"))
				Reset();
		}
		ImGui::End();
	}

	void Camera::SetFOV(float NewFOV)
	{
		m_FOV = NewFOV;
	}

	float Camera::GetFOV() const
	{
		return m_FOV;
	}

	void Camera::SetProjection(DirectX::XMMATRIX Projection)
	{
		Projection_ = Projection;
	}

	DirectX::XMMATRIX Camera::GetProjection() const
	{
		return Projection_;
	}

	DirectX::XMFLOAT3 Camera::GetPosition() const
	{
		return Position_.ToDXFloat3();
	}
}