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

	IntVector4(int x, int y, int z, int w)
		: x(x), y(y), z(z), w(w)
	{}

	IntVector4(int xyzw)
		: x(xyzw), y(xyzw), z(xyzw), w(xyzw)
	{}

	IntVector4(Vector4 Input)
		: x(static_cast<int>(Input.x)), y(static_cast<int>(Input.y)), z(static_cast<int>(Input.z)), w(static_cast<int>(Input.w))
	{}

	//= Operators =========================================================



	//= Public Variables ===========================

	int x;
	int y;
	int z;
	int w;
};

//= External Inline Functions =======================================
