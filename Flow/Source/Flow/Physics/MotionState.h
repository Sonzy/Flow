#pragma once
#include "ThirdParty/Bullet/LinearMath/btMotionState.h"
#include "Maths/Maths.h"

class MotionState : public btMotionState
{
public:
	MotionState() = delete;
	MotionState(Vector3* Position, Rotator* Rotation);
	void UpdateTransformPointers(Vector3* Position, Rotator* Rotation);

	virtual void getWorldTransform(btTransform& worldTrans) const override;
	virtual void setWorldTransform(const btTransform& worldTrans) override;

protected:

	Vector3* _ObjectPosition;
	Rotator* _ObjectRotation;
};