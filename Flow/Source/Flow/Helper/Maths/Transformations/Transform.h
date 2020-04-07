#pragma once
#include "Vector.h"
#include "Rotator.h"

class Transform
{
public:
	Transform()
		: Position_(Vector()), Rotation_(Rotator()), Scale_(Vector(1.0f))
	{}

	Transform(Vector Loc, Rotator Rot, Vector Scale)
		: Position_(Loc), Rotation_(Rot), Scale_(Scale)
	{}

	Vector Position_;
	Rotator Rotation_;
	Vector Scale_;

};