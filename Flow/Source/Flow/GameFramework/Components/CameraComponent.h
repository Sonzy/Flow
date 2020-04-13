#pragma once
#include "Flow\GameFramework\Components\WorldComponent.h"
#include <DirectXMath.h>
#include "Flow\Helper\Maths.h"

namespace Flow
{
	class FLOW_API CameraComponent : public WorldComponent
	{
	public:
		CameraComponent();
		CameraComponent(const std::string& Name);

		virtual void Tick(float DeltaTime);

		void SetFOV(float FOV);
		void SetProjection(DirectX::XMMATRIX Projection);


		DirectX::XMMATRIX GetViewMatrix() const;
		DirectX::XMMATRIX GetProjectionMatrix() const;
		float GetFOV() const;

	private:

		IntVector2D LastMousePosition_;
		DirectX::XMMATRIX Projection_;
		float FOV_;

		float MovementSpeed_;
		float RotationSpeed_;
	};
}
