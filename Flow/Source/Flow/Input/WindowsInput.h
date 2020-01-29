#pragma once
#include "Input.h"

namespace Flow
{
	// Dont need flow API marker since included in input
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressed_Implementation(int Keycode);
	};
}