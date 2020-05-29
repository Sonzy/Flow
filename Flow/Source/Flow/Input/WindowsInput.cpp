#include "Flowpch.h"
#include "WindowsInput.h"

Input* Input::s_Instance = new WindowsInput();

bool WindowsInput::IsKeyPressed_Implementation(int Keycode)
{
	return _InputStates[Keycode];
}

