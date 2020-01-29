#include "Flowpch.h"
#include "WindowsInput.h"

namespace Flow
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressed_Implementation(int Keycode)
	{
		return InputStates[Keycode];
	}
}

