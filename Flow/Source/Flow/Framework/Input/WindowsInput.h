#pragma once

//= Includes =================================

#include "Input.h"

//= Class Definition =========================

class WindowsInput : public Input
{
protected:

	//= Protected Functions ===============

	virtual bool IsKeyPressed_Implementation(int Keycode);
};