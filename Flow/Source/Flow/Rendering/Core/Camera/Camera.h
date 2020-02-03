#pragma once
#include <DirectXMath.h>
#include "Flow/Helper/Maths.h"

namespace Flow
{
	class Camera
	{
	public:
		Camera();

		DirectX::XMMATRIX GetMatrix() const;

		void Reset();
		void Translate(DirectX::XMFLOAT3 Translation);
		void SetRotation(Rotator Rotation);
		void Rotate(Rotator Offset);

		void Tick(float DeltaTime);

		void RenderIMGUIWindow();

		void SetFOV(float NewFOV);
		float GetFOV() const;

		void SetProjection(DirectX::XMMATRIX Projection);
		DirectX::XMMATRIX GetProjection() const;

		DirectX::XMFLOAT3 GetPosition() const;

	private:

		Vector m_Position;
		Rotator m_Rotation; //Pitch Roll Yaw

		IntVector2D m_LastMousePos;

		DirectX::XMMATRIX m_Projection;

		float m_FOV = Math::PI / 2;

		float m_CameraSpeed = 1.0f;
		float m_RotationSpeed = 0.005f;
	};
}