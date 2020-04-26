#pragma once
#include "Camera.h"
#include "Flow\Core.h"
#include "Flow\Helper\Maths.h"

namespace Flow
{
	class FLOW_API BasicCamera : public CameraBase
	{
	public:
		BasicCamera();

		[[nodiscard]] DirectX::XMMATRIX GetView() const override;

		void Reset();
		void Translate(DirectX::XMFLOAT3 Translation);
		void SetRotation(Rotator Rotation);
		void Rotate(Rotator Offset);

		virtual void Update(float DeltaTime) override;

		void RenderIMGUIWindow();

		Vector GetCameraPosition() const override;

	private:

		Vector Position_;
		Rotator Rotation_;

		IntVector2D LastMousePosition_;

		float CameraSpeed_ = 0.2f;
		float RotationSpeed_ = 0.005f;
	};
}