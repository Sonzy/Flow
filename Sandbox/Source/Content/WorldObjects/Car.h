#pragma once
#include "Flow/GameFramework/Pawn.h"

class StaticMeshComponent;

class Car : public Pawn
{
public:
	Car();
	~Car();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:

	std::shared_ptr<StaticMeshComponent> _Body;
	std::shared_ptr<StaticMeshComponent> _Wheel_FrontLeft;
	std::shared_ptr<StaticMeshComponent> _Wheel_FrontRight;
	std::shared_ptr<StaticMeshComponent> _Wheel_BackLeft;
	std::shared_ptr<StaticMeshComponent> _Wheel_BackRight;
};