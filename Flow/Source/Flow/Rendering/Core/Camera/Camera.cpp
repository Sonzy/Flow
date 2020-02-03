#include "Flowpch.h"
#include "Camera.h"
#include "Flow/Helper/Maths.h"
#include "ThirdParty/ImGui/imgui.h"
#include <algorithm>

namespace Flow
{
	Camera::Camera()
		: m_Position(0.0f), m_Rotation(0.0f)
	{
		Reset();
	}

	DirectX::XMMATRIX Camera::GetMatrix() const
	{
		const DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		
		//Apply rotations to the base vector
		const auto lookVector = DirectX::XMVector3Transform(Forward,
			DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.Pitch, m_Rotation.Yaw, m_Rotation.Roll) //TODO: Rotation
		);
		
		
		// Generate a camera transform
		DirectX::XMFLOAT3 fLookVector;
		DirectX::XMFLOAT3 camTarget;
		
		DirectX::XMStoreFloat3(&fLookVector, lookVector);
		camTarget.x = m_Position.X + fLookVector.x;
		camTarget.y = m_Position.Y + fLookVector.y;
		camTarget.z = m_Position.Z + fLookVector.z;
		
		DirectX::XMVECTOR vCamTarget = DirectX::XMLoadFloat3(&camTarget);
		DirectX::XMVECTOR camPosition = DirectX::XMLoadFloat3(&m_Position.ToDXFloat3());
		return DirectX::XMMatrixLookAtLH(camPosition, vCamTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	}

	void Camera::Reset()
	{
		m_Position = { 0.0f, 0.0f, 0.0f };

		m_Rotation.Pitch = 0.0f;
		m_Rotation.Roll = 0.0f;
	}

	void Camera::Translate(DirectX::XMFLOAT3 Translation)
	{
		DirectX::XMStoreFloat3(&Translation, DirectX::XMVector3Transform(
			DirectX::XMLoadFloat3(&Translation),
			DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.Pitch, m_Rotation.Yaw, m_Rotation.Roll) *
			DirectX::XMMatrixScaling(m_CameraSpeed, m_CameraSpeed, m_CameraSpeed)
		));

		m_Position.X += Translation.x;
		m_Position.Y += Translation.y;
		m_Position.Z += Translation.z;
	}

	void Camera::Rotate(Rotator Rotation)
	{
		m_Rotation.Yaw = Math::WrapAngle(m_Rotation.Yaw + Rotation.Yaw * m_RotationSpeed);
		m_Rotation.Pitch = std::clamp(m_Rotation.Pitch + Rotation.Pitch * m_RotationSpeed, .995f * -PI / 2.0f, .995f * PI / 2.0f);
	}

	void Camera::RenderIMGUIWindow()
	{
		if (ImGui::Begin("Camera"))
		{
			ImGui::Text("Position");
			ImGui::SliderFloat("X", &m_Position.X, -80.0f, 80.0f, "%.1f");
			ImGui::SliderAngle("Y", &m_Position.Y, -80.0f, 80.0f, "%.1f");
			ImGui::SliderAngle("Z", &m_Position.Z, -80.0f, 80.0f, "%.1f");
			ImGui::Text("Rotation");
			ImGui::SliderAngle("Pitch", &m_Rotation.Pitch, 0.995f * -90.0f, 0.995f * 90.0f);
			ImGui::SliderAngle("Yaw", &m_Rotation.Yaw, -180.0f, 180.0f);
		
			if (ImGui::Button("Reset"))
				Reset();
		}
		ImGui::End();
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