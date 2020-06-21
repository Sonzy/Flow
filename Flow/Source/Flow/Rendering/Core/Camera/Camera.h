#pragma once
#include <DirectXMath.h>
#include "Flow/Helper/Maths.h"

class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class MouseScrolledEvent;
class KeyPressedEvent;
class KeyReleasedEvent;

/* Interface for camera, anything inheriting from this can be used as a camera.
Use Update instead of tick, if the camera is moved outside of update, re-cache */
class FLOW_API CameraBase
{
public:
	virtual void SetProjectionMatrix(DirectX::XMMATRIX NewProjection);

#if WITH_EDITOR
	void SetSceneProjection(DirectX::XMMATRIX NewProjection);
	DirectX::XMMATRIX GetSceneProjectionMatrix() const;
#endif

	[[nodiscard]] virtual DirectX::XMMATRIX GetViewMatrix() const = 0;
	virtual void MoveCamera(const Transform& NewTransform) = 0;
	virtual Transform GetCameraTransform() const = 0;

	virtual bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	virtual bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
	virtual bool OnMouseMoved(MouseMovedEvent& e);
	virtual bool OnMouseScrolled(MouseScrolledEvent& e);
	virtual bool OnKeyPressed(KeyPressedEvent& e);
	virtual bool OnKeyReleased(KeyReleasedEvent& e);

	//TODO: Not sure whether to keep this here
	virtual void Update(float DeltaTime);

	virtual void CacheMatrices();

	DirectX::XMMATRIX GetCachedViewProjection();
	DirectX::XMMATRIX GetTransposedCachedViewProjection();
	DirectX::XMMATRIX GetCachedView();

	DirectX::XMMATRIX GetProjectionMatrix() const;

	virtual Vector GetCameraPosition() const = 0;

	virtual void SetFOV(float NewFOV) { _FieldOfView = NewFOV; }
	virtual float GetFOV() const { return _FieldOfView; };

	void MarkCacheDirty() { _CacheDirty = true; }

protected:

	DirectX::XMMATRIX _CachedViewProj;
	DirectX::XMMATRIX _CachedView;
	DirectX::XMMATRIX _CachedTransposedViewProj;
	DirectX::XMMATRIX _Projection;

#if WITH_EDITOR
	DirectX::XMMATRIX _SceneProjection;
#endif

	bool _CacheDirty = false;
	Vector _LastKnownCameraPos;

	float _FieldOfView = 90;
};