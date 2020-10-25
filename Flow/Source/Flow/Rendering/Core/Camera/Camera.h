#pragma once

//= Includes =================================================

#include <DirectXMath.h>
#include "Maths/Maths.h"

//= Forward Declarations =====================================

class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class MouseScrolledEvent;
class KeyPressedEvent;
class KeyReleasedEvent;

//= Class Definition =========================================

/* Interface for camera, anything inheriting from this can be used as a camera.
Use Update instead of tick, if the camera is moved outside of update, re-cache */
class FLOW_API CameraBase
{
public:

	//= Public Functions
	virtual void							SetProjectionMatrix(DirectX::XMMATRIX NewProjection);
	N_DISC virtual DirectX::XMMATRIX		GetViewMatrix() const = 0;
	virtual void							MoveCamera(const Transform& NewTransform) = 0;
	virtual Transform						GetCameraTransform() const = 0;

	virtual bool							OnMouseButtonPressed(MouseButtonPressedEvent& e);
	virtual bool							OnMouseButtonReleased(MouseButtonReleasedEvent& e);
	virtual bool							OnMouseMoved(MouseMovedEvent& e);
	virtual bool							OnMouseScrolled(MouseScrolledEvent& e);
	virtual bool							OnKeyPressed(KeyPressedEvent& e);
	virtual bool							OnKeyReleased(KeyReleasedEvent& e);

	//TODO: Not sure whether to keep this here
	virtual void							Update(float DeltaTime);
	virtual void							CacheMatrices();

	DirectX::XMMATRIX						GetCachedViewProjection();
	DirectX::XMMATRIX						GetTransposedCachedViewProjection();
	DirectX::XMMATRIX						GetCachedView();
	DirectX::XMMATRIX						GetProjectionMatrix() const;
	virtual Vector3							GetCameraPosition() const = 0;

	virtual void							SetFOV(float NewFOV)	{ m_FieldOfView = NewFOV; }
	virtual float							GetFOV() const			{ return m_FieldOfView; };

	void									MarkCacheDirty()		{ m_CacheDirty = true; }

#if WITH_EDITOR
	void									SetSceneProjection(DirectX::XMMATRIX NewProjection);
	DirectX::XMMATRIX						GetSceneProjectionMatrix() const;
#endif

protected:

	//= Protected Variables =========================================

	DirectX::XMMATRIX						m_CachedViewProj;
	DirectX::XMMATRIX						m_CachedView;
	DirectX::XMMATRIX						m_CachedTransposedViewProj;
	DirectX::XMMATRIX						m_Projection;											
#if WITH_EDITOR								
	DirectX::XMMATRIX						m_SceneProjection;
#endif

	bool									m_CacheDirty = false;
	Vector3									m_LastKnownCameraPos;
	Vector3									m_Forward;
	float									m_FieldOfView = 90;
};