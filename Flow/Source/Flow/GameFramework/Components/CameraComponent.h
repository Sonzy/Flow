#pragma once

// Includes ////////////////////////////////////////////////////////////////

#include <DirectXMath.h>
#include "GameFramework/Components/WorldComponent.h"
#include "Rendering/Core/Camera/Camera.h"

// Forward Declarations ////////////////////////////////////////////////////

namespace YAML { class Emitter; }

// Class Definitions ///////////////////////////////////////////////////////

/* Component wrapper around the camera interface. */
class FLOW_API CameraComponent : public WorldComponent, public CameraBase
{
public:

	REGISTER_CLASS(CameraComponent)
	
	// Public Functions ///////////////////////////////////////////////////

									CameraComponent();
									CameraComponent(const std::string& Name);

	virtual void					OnRegistered() override;

#if WITH_EDITOR
	virtual void					IconUpdate(IconManager& iconManager) override;
#endif

	virtual void					Update(float DeltaTime) override;
	virtual void					MoveCamera(const Transform& NewTransform) override;
	virtual Transform				GetCameraTransform() const override;

#if WITH_EDITOR
	virtual void					CustomRender() override;
#endif

	// Camera Interface //

	virtual DirectX::XMMATRIX		GetViewMatrix() const override;
	virtual Vector3					GetCameraPosition() const override;
};