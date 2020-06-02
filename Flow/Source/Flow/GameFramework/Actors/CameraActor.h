#pragma once
#include "Flow/GameFramework/Pawn.h"

class CameraComponent;

class FLOW_API CameraActor : public Pawn
{
public:
	CameraActor();
	CameraActor(const std::string& Name);

	virtual void OnControlled(Controller* OwningController) override;
	virtual void Tick(float DeltaTime) override;

	void SetMainCamera() const;

protected:

	std::shared_ptr<CameraComponent> _CameraComponent;
	IntVector2D _LastMousePosition;

	float CameraMovementSpeed;
	float CameraHorizontalRotationSpeed;
	float CameraVerticalRotationSpeed;
};
