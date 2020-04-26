#include "Flowpch.h"
#include "BasicCamera.h"
#include "Flow\Input\Input.h"
#include "ThirdParty/ImGui/imgui.h"

namespace Flow
{
	BasicCamera::BasicCamera()
		: Position_(2.5f, 3.5f, -5.0f), Rotation_(0.0f), LastMousePosition_(Input::GetMousePosition())
	{
		SetFOV(90.0f);
	}

	DirectX::XMMATRIX BasicCamera::GetView() const
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

	void BasicCamera::Reset()
	{
		Position_ = { 0.0f, 0.0f, -10.0f };

		Rotation_.Pitch = 0.0f;
		Rotation_.Roll = 0.0f;
	}

	void BasicCamera::Translate(DirectX::XMFLOAT3 Translation)
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

	void BasicCamera::SetRotation(Rotator Rotation)
	{
		Rotation_ = Rotation;
	}

	void BasicCamera::Rotate(Rotator Offset)
	{
		Rotation_ += Offset;
	}

	void BasicCamera::Update(float DeltaTime)
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

		CacheCameraMatrices();
	}

	void BasicCamera::RenderIMGUIWindow()
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
			ImGui::SliderFloat("Camera Speed", &CameraSpeed_, 0.1f, 2.0f, "%.1f");
			ImGui::SliderAngle("Field Of View", &FieldOfView_, 60.0f, 100.0f);

			if (ImGui::Button("Reset"))
				Reset();
		}
		ImGui::End();
	}

	Vector BasicCamera::GetCameraPosition() const
	{
		return Position_;
	}
}