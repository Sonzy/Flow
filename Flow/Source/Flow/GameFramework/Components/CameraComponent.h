#include "Flow\GameFramework\Components\WorldComponent.h"
#include <DirectXMath.h>
#include "Flow\Rendering\Core\Camera\Camera.h"

/* Component wrapper around the camera interface. */
class FLOW_API CameraComponent : public WorldComponent, public CameraBase
{
public:
	CameraComponent();
	CameraComponent(const std::string& Name);

	virtual void Update(float DeltaTime) override;
	virtual void MoveCamera(const Transform& NewTransform) override;
	virtual Transform GetCameraTransform() const override;


	// === ICamera interface ===

	[[nodiscard]] virtual DirectX::XMMATRIX GetViewMatrix() const override;
	virtual Vector3 GetCameraPosition() const override;

};