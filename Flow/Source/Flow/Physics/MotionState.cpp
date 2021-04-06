#include "pch.h"
#include "MotionState.h"

MotionState::MotionState(Vector3* Position, Rotator* Rotation)
	: m_ObjectPosition(Position), m_ObjectRotation(Rotation)
{
}

void MotionState::UpdateTransformPointers(Vector3* Position, Rotator* Rotation)
{
	m_ObjectPosition = Position;
	m_ObjectRotation = Rotation;
}

void MotionState::getWorldTransform(btTransform& worldTrans) const
{
	worldTrans.setOrigin(btVector3(m_ObjectPosition->x, m_ObjectPosition->y, m_ObjectPosition->z));

	//Convert to radians for now, need to do rotations better lul
	btQuaternion Rotation;
	Rotator RadiansRotation = Rotator::AsRadians(*m_ObjectRotation);
	Rotation.setEuler(RadiansRotation.Yaw, RadiansRotation.Pitch, RadiansRotation.Roll);
	worldTrans.setRotation(Rotation);
}

void MotionState::setWorldTransform(const btTransform& worldTrans)
{
	//We assume that the object is the root component
	btVector3 Vec = worldTrans.getOrigin();
	m_ObjectPosition->x = Vec.x();
	m_ObjectPosition->y = Vec.y();
	m_ObjectPosition->z = Vec.z();

	btScalar m[16];
	worldTrans.getOpenGLMatrix(m);
	float fAngZ = atan2f(m[1], m[5]);
	float fAngY = atan2f(m[8], m[10]);
	float fAngX = -asinf(m[9]);

	//To Degrees
	float Modifier = 180 / Maths::PI;
	m_ObjectRotation->Pitch = fAngX * Modifier;
	m_ObjectRotation->Roll = fAngZ * Modifier;
	m_ObjectRotation->Yaw = fAngY * Modifier;
}
