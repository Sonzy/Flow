#include "Flow\GameFramework\Components\WorldComponent.h"
#include <DirectXMath.h>
#include "Flow\Helper\Maths.h"
#include "Flow\Rendering\Core\Camera\Camera.h"

/* Component wrapper around the camera interface. */
class FLOW_API CameraComponent : public WorldComponent, public CameraBase
{
public:
	CameraComponent();
	CameraComponent(const std::string& Name);

	virtual void Update(float DeltaTime) override;

	// === ICamera interface ===

	[[nodiscard]] virtual DirectX::XMMATRIX GetViewMatrix() const override;
	virtual Vector GetCameraPosition() const override;

private:

	IntVector2D _LastMousePosition;

	float _MovementSpeed;
	float _RotationSpeed;
};