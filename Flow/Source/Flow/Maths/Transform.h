#pragma once

//= Includes ===========================================

#include "Vector3.h"
#include "Rotator.h"

//= Class Definition ===================================

class FLOW_API Transform
{
public:

	//= Public Functions ==================================================

	Transform()
		: m_Position(Vector3(0.0f)), m_Rotation(Rotator(0.0f)), m_Scale(Vector3(1.0f))
	{}

	Transform(Vector3 Loc, Rotator Rot, Vector3 Scale)
		: m_Position(Loc), m_Rotation(Rot), m_Scale(Scale)
	{}

public:

	//= Public Variables ===========================

	Vector3 m_Position;
	Rotator m_Rotation;
	Vector3 m_Scale;
};