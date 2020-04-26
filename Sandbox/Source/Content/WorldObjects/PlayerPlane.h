#pragma once
#include "Flow\GameFramework\ControlledObject.h"

namespace Flow
{
	class StaticMeshComponent;
	class CameraComponent;
	class WorldComponent;
}

class PlayerPlane : public Flow::ControlledObject
{
public:
	PlayerPlane();
	PlayerPlane(const std::string& Name);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void DrawDetailsWindow(bool bDontUpdate) override;

	//void OnObjectHitPlane(Flow::WorldComponent* Comp1, Flow::WorldComponent* Comp2);
	virtual void OnComponentCollision(Flow::WorldComponent* Component, Flow::WorldComponent* Other) override;

	void KillPlane();

	void Test(bool a);

	void ToggleWASDMode();

	float PlaneSpeed_ = 0.25f;



	bool WASDMode = false;

	IntVector2D LastMousePosition_;

protected:
	bool Alive;

	std::shared_ptr<Flow::StaticMeshComponent> PlaneMesh_;
	std::shared_ptr<Flow::CameraComponent> PlayerCamera_;
};