#pragma once
#include "Flow\GameFramework\Components\WorldComponent.h"
#include <DirectXMath.h>
#include "Flow\Helper\Maths.h"
#include "Flow\Rendering\Core\Camera\Camera.h"

namespace Flow
{
	/* Component wrapper around the camera interface. */
	class FLOW_API CameraComponent : public WorldComponent, public CameraBase
	{
	public:
		CameraComponent();
		CameraComponent(const std::string& Name);

		// Dont use in camera
		virtual void Tick(float DeltaTime);

		virtual void Update(float DeltaTime) override;
		
		// === ICamera interface ===

		[[nodiscard]] virtual DirectX::XMMATRIX GetView() const override;

		virtual Vector GetCameraPosition() const override;

	private:

		IntVector2D LastMousePosition_;

		float MovementSpeed_;
		float RotationSpeed_;
	};
}
