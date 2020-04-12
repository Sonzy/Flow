#pragma once
#include <DirectXMath.h>
#include "Flow/Helper/Maths.h"

namespace Flow
{
	class FLOW_API Camera
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

		Vector Position_;
		Rotator Rotation_; //Pitch Roll Yaw

		IntVector2D LastMousePosition_;

		DirectX::XMMATRIX Projection_;

		float m_FOV = Math::PI / 2;

		float CameraSpeed_ = 0.2f;
		float RotationSpeed_ = 0.005f;
	};
}