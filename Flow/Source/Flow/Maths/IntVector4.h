#pragma once

//= Includes ===========================================

#include "Vector4.h"

//= Class Definition ===================================

class FLOW_API IntVector4
{
public:

	//= Constructors ==================================================

	IntVector4()
		: x(0), y(0), z(0), w(0)
	{}

	IntVector4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{}

	IntVector4(float xyzw)
		: x(xyzw), y(xyzw), z(xyzw), w(xyzw)
	{}

	IntVector4(Vector4 Input)
		: x(Input.z), y(Input.y), z(Input.z), w(0)
	{}

	//= Operators =========================================================



	//= Public Variables ===========================

	float x;
	float y;
	float z;
	float w;
};

//= External Inline Functions =======================================
