#pragma once
#include "Flow\GameFramework\ControlledObject.h"

namespace Flow
{
	class StaticMeshComponent;
	class CameraComponent;
}

class PlayerPlane : public Flow::ControlledObject
{
public:
	PlayerPlane();
	PlayerPlane(const std::string& Name);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	float PlaneSpeed_ = 0.25f;

protected:

	std::shared_ptr<Flow::StaticMeshComponent> PlaneMesh_;
	std::shared_ptr<Flow::CameraComponent> PlayerCamera_;
};