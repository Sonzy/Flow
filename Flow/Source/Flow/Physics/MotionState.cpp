#include "Flowpch.h"
#include "MotionState.h"

MotionState::MotionState(Vector* Position, Rotator* Rotation)
	: _ObjectPosition(Position), _ObjectRotation(Rotation)
{
}

void MotionState::UpdateTransformPointers(Vector* Position, Rotator* Rotation)
{
	_ObjectPosition = Position;
	_ObjectRotation = Rotation;
}

void MotionState::getWorldTransform(btTransform& worldTrans) const
{
	worldTrans.setOrigin(btVector3(_ObjectPosition->X, _ObjectPosition->Y, _ObjectPosition->Z));

	//Convert to radians for now, need to do rotations better lul
	btQuaternion Rotation;
	Rotator RadiansRotation = Rotator::AsRadians(*_ObjectRotation);
	Rotation.setEuler(RadiansRotation.Yaw, RadiansRotation.Pitch, RadiansRotation.Roll);
	worldTrans.setRotation(Rotation);
}

void MotionState::setWorldTransform(const btTransform& worldTrans)
{
	//We assume that the object is the root component
	btVector3 Vec = worldTrans.getOrigin();
	_ObjectPosition->X = Vec.x();
	_ObjectPosition->Y = Vec.y();
	_ObjectPosition->Z = Vec.z();

	btScalar m[16];
	worldTrans.getOpenGLMatrix(m);
	float fAngZ = atan2f(m[1], m[5]);
	float fAngY = atan2f(m[8], m[10]);
	float fAngX = -asinf(m[9]);

	//To Degrees
	float Modifier = 180 / Math::PI;
	_ObjectRotation->Pitch = fAngX * Modifier;
	_ObjectRotation->Roll = fAngZ * Modifier;
	_ObjectRotation->Yaw = fAngY * Modifier;
}
