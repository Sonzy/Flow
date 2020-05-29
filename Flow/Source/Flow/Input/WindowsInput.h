#pragma once
#include "Input.h"

// Dont need flow API marker since included in input
class WindowsInput : public Input
{
protected:
	virtual bool IsKeyPressed_Implementation(int Keycode);
};