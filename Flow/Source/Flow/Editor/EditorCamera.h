#pragma once
#include "Flow/Rendering/Core/Camera/Camera.h"

class EditorCamera : public CameraBase
{
public:
	EditorCamera();

	[[nodiscard]] virtual DirectX::XMMATRIX GetViewMatrix() const override;
	virtual void MoveCamera(const Transform& NewTransform) override;
	virtual Transform GetCameraTransform() const override;
	virtual Vector3 GetCameraPosition() const override;

	virtual void Update(float DeltaTime) override;

protected:
	friend class EditorLayer;
	friend class SceneManager;

	bool _CanUpdate;
	IntVector2 _MouseLastFrame;

	Vector3 _Position;
	Rotator _Rotation;


	float _CameraSpeed;
};