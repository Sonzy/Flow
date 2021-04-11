#pragma once

//= Includes =========================

#include <DirectXMath.h>

//= Struct Definitions ===============

/* Constant buffer template class, storing 4 32bit floats */
struct CBT_F32F32F32F32
{
	CBT_F32F32F32F32()
		: R(0.0f), G(0.0f), B(0.0f), A(0.0f)
	{

	}

	CBT_F32F32F32F32(float R, float G, float B, float A)
		: R(R), G(G), B(B), A(A)
	{

	}

	float R;
	float G;
	float B;
	float A;
};

typedef CBT_F32F32F32F32 CBT_Colour;