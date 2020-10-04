#pragma once

//= Includes =============================================

#include "Flow/Rendering/Core/Camera/Camera.h"

//= Class Definition =====================================

class EditorCamera : public CameraBase
{
public:
	
	//= Public Functions =================================
	EditorCamera();

	virtual void								MoveCamera(const Transform& NewTransform) override;
	virtual void								Update(float DeltaTime) override;

	N_DISC virtual DirectX::XMMATRIX				GetViewMatrix() const override;
	virtual Transform							GetCameraTransform() const override;
	virtual Vector3								GetCameraPosition() const override;


protected:
	friend class Editor;
	friend class SceneManager;

	//= Protected Variables ==============================

	bool										m_CanUpdate;
	IntVector2									m_MouseLastFrame;
	Vector3										m_Position;
	Rotator										m_Rotation;
	float										m_CameraSpeed;
};