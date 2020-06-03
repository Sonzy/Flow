#pragma once
#include "ThirdParty/Bullet/LinearMath/btMotionState.h"
#include "Flow/Helper/Maths.h"

class MotionState : public btMotionState
{
public:
	MotionState() = delete;
	MotionState(Vector* Position, Rotator* Rotation);
	void UpdateTransformPointers(Vector* Position, Rotator* Rotation);

	virtual void getWorldTransform(btTransform& worldTrans) const override;
	virtual void setWorldTransform(const btTransform& worldTrans) override;

protected:

	Vector* _ObjectPosition;
	Rotator* _ObjectRotation;
};