#pragma once
#include "Flow/Core.h"
#include <bitset>
#include "KeyCodes.h"
#include "Maths\IntVector2.h"

class FLOW_API Input
{
public:

	static bool IsKeyPressed(int Keycode)
	{
		return s_Instance->IsKeyPressed_Implementation(Keycode);
	}

	static bool IsMousePressed(int MouseButton)
	{
		return s_Instance->_InputStates[MouseButton];
	}

	static void OnKeyPressed(int KeyCode)
	{
		s_Instance->_InputStates[KeyCode] = true;
	}

	static void OnKeyReleased(int KeyCode)
	{
		s_Instance->_InputStates[KeyCode] = false;
	}

	static void OnMouseMoved(int X, int Y)
	{
		s_Instance->_MouseX = X;
		s_Instance->_MouseY = Y;
	}

	static int GetMouseX()
	{
		return s_Instance->_MouseX;
	}

	static int GetMouseY()
	{
		return s_Instance->_MouseY;
	}

	static IntVector2 GetMousePosition()
	{
		return IntVector2(s_Instance->_MouseX, s_Instance->_MouseY);
	}


protected:
	virtual bool IsKeyPressed_Implementation(int Keycode) = 0;

	std::bitset<256> _InputStates;

	int _MouseX;
	int _MouseY;
private:


	static Input* s_Instance;
};