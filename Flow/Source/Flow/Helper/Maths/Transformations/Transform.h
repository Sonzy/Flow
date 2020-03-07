#pragma once
#include "Vector.h"
#include "Rotator.h"

class Transform
{
public:
	Transform()
		: m_Location(Vector()), m_Rotation(Rotator()), m_Scale(Vector(1.0f))
	{}

	Transform(Vector Loc, Rotator Rot, Vector Scale)
		: m_Location(Loc), m_Rotation(Rot), m_Scale(Scale)
	{}

	Vector m_Location;
	Rotator m_Rotation;
	Vector m_Scale;

};