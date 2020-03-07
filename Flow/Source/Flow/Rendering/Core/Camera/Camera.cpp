#include "Flowpch.h"
#include "Camera.h"
#include "Flow/Helper/Maths.h"
#include "ThirdParty/ImGui/imgui.h"
#include <algorithm>
#include "Flow\Input\Input.h"

namespace Flow
{
	Camera::Camera()
		: m_Position(0.0f,0.0f, -100.0f), m_Rotation(0.0f), m_LastMousePos(Input::GetMousePosition())
	{
	}

	DirectX::XMMATRIX Camera::GetMatrix() const
	{
		const DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		
		//Get Camera Look Vector
		const auto lookVector = DirectX::XMVector3Transform(Forward,
			DirectX::XMMatrixRotationRollPitchYaw(Math::DegreesToRadians(m_Rotation.Pitch), Math::DegreesToRadians(m_Rotation.Yaw), Math::DegreesToRadians(m_Rotation.Roll))
		);
		
		//Get the location the camera is looking
		DirectX::XMFLOAT3 fLookVector;
		DirectX::XMFLOAT3 camTarget;
		
		DirectX::XMStoreFloat3(&fLookVector, lookVector);
		camTarget.x = m_Position.X + fLookVector.x;
		camTarget.y = m_Position.Y + fLookVector.y;
		camTarget.z = m_Position.Z + fLookVector.z;
		
		//Return transformed Matrix with camera looking at a target
		DirectX::XMVECTOR vCamTarget = DirectX::XMLoadFloat3(&camTarget);
		DirectX::XMVECTOR camPosition = DirectX::XMLoadFloat3(&m_Position.ToDXFloat3());
		return DirectX::XMMatrixLookAtLH(camPosition, vCamTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	}

	void Camera::Reset()
	{
		m_Position = { 0.0f, 0.0f, -10.0f };

		m_Rotation.Pitch = 0.0f;
		m_Rotation.Roll = 0.0f;
	}

	void Camera::Translate(DirectX::XMFLOAT3 Translation)
	{
		DirectX::XMStoreFloat3(&Translation, DirectX::XMVector3Transform(
			DirectX::XMLoadFloat3(&Translation),
			DirectX::XMMatrixRotationRollPitchYaw(Math::DegreesToRadians(m_Rotation.Pitch), Math::DegreesToRadians(m_Rotation.Yaw), Math::DegreesToRadians(m_Rotation.Roll)) *
			DirectX::XMMatrixScaling(m_CameraSpeed, m_CameraSpeed, m_CameraSpeed)
		));

		m_Position.X += Translation.x;
		m_Position.Y += Translation.y;
		m_Position.Z += Translation.z;
	}

	void Camera::SetRotation(Rotator Rotation)
	{
		m_Rotation = Rotation;
	}

	void Camera::Rotate(Rotator Offset)
	{
		m_Rotation += Offset;
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
			if (m_LastMousePos != Pos)
			{
				Vector Direction = (m_LastMousePos - Pos);
				Direction.X = Direction.X * 0.25f; //Horizontal Sensitivity
				Direction.Y = Direction.Y * 0.15f;  //Vertical Sensitivity
				Rotate(Rotator(-Direction.Y, 0.0f, -Direction.X));
			}
		}

		Translate(Translation.ToDXFloat3());

		m_LastMousePos = Pos;
	}

	void Camera::RenderIMGUIWindow()
	{
		if (ImGui::Begin("Camera"))
		{
			ImGui::Text("Position");
			ImGui::SliderFloat("X", &m_Position.X, -200.0f, 200.0f, "%.1f");
			ImGui::SliderFloat("Y", &m_Position.Y, -200.0f, 200.0f, "%.1f");
			ImGui::SliderFloat("Z", &m_Position.Z, -200.0f, 200.0f, "%.1f");
			ImGui::Text("Rotation");
			ImGui::Text("Pitch: %f", m_Rotation.Pitch);
			ImGui::Text("Roll: %f", m_Rotation.Roll);
			ImGui::Text("Yaw: %f", m_Rotation.Yaw);
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
		m_Projection = Projection;
	}

	DirectX::XMMATRIX Camera::GetProjection() const
	{
		return m_Projection;
	}

	DirectX::XMFLOAT3 Camera::GetPosition() const
	{
		return m_Position.ToDXFloat3();
	}
}