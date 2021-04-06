#include "pch.h"
#include "WindowsInput.h"

Input* Input::sm_Instance = new WindowsInput();

bool WindowsInput::IsKeyPressed_Implementation(int Keycode)
{
	return m_InputStates[Keycode];
}

