#pragma once
#include "Vector.h"
#include "Rotator.h"

class Transform
{
public:
	Transform()
		: _Position(Vector()), _Rotation(Rotator()), _Scale(Vector(1.0f))
	{}

	Transform(Vector Loc, Rotator Rot, Vector Scale)
		: _Position(Loc), _Rotation(Rot), _Scale(Scale)
	{}

	Vector _Position;
	Rotator _Rotation;
	Vector _Scale;

};