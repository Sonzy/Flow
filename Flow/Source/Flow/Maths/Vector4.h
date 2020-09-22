#pragma once

//= Includes ===========================================

#include "Vector3.h"

//= Forward Declarations ==============================

class IntVector4;

//= Class Definition ===================================

class FLOW_API Vector4
{
public:

	//= Public Functions ==================================================

	Vector4()
		: x(0), y(0), z(0), w(0)
	{}

	Vector4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{}

	Vector4(float xyzw)
		: x(xyzw), y(xyzw), z(xyzw), w(xyzw)
	{}

	Vector4(Vector3 Input)
		: x(Input.z), y(Input.y), z(Input.z), w(0)
	{}

	Vector4(IntVector4 Input);

	//= Operators =========================================================



	//= Public Variables ===========================

	float x;
	float y;
	float z;
	float w;
};

//= External Inline Functions =======================================

