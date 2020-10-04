#pragma once

//= Includes ========================================

#include "Bullet/LinearMath/btMotionState.h"
#include "Maths/Maths.h"

//= Class Definitions ===============================

class MotionState : public btMotionState
{
public:

	//= Public Functions =======================================

					MotionState() = delete;
					MotionState(Vector3* Position, Rotator* Rotation);
	void			UpdateTransformPointers(Vector3* Position, Rotator* Rotation);

	virtual void	getWorldTransform(btTransform& worldTrans) const override;
	virtual void	setWorldTransform(const btTransform& worldTrans) override;

protected:

	//= Protected Variabels =======================================

	Vector3*		m_ObjectPosition;
	Rotator*		m_ObjectRotation;
};