#pragma once

//= Includes =========================================

#include "Maths/Vector3.h"

//= Class Definition =================================

class DebugDraw
{
public:

	static void DrawLine(Vector3 Start, Vector3 End, Vector3 Color = Vector3(1.0f, 1.0f, 1.0f));
	static void DrawBox(Vector3 Centre, float Extents);
};