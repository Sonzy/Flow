#pragma once
#include "Flow/Core.h"
#include <bitset>
#include "KeyCodes.h"
#include "Maths\IntVector2.h"

class FLOW_API Input
{
public:

	//= Public Static Functions =================================

	static bool IsKeyPressed(int Keycode)
	{
		return sm_Instance->IsKeyPressed_Implementation(Keycode);
	}

	static bool IsMousePressed(int MouseButton)
	{
		return sm_Instance->m_InputStates[MouseButton];
	}

	static void OnKeyPressed(int KeyCode)
	{
		sm_Instance->m_InputStates[KeyCode] = true;
	}

	static void OnKeyReleased(int KeyCode)
	{
		sm_Instance->m_InputStates[KeyCode] = false;
	}

	static void OnMouseMoved(int X, int Y)
	{
		sm_Instance->m_MouseX = X;
		sm_Instance->m_MouseY = Y;
	}

	static int GetMouseX()
	{
		return sm_Instance->m_MouseX;
	}

	static int GetMouseY()
	{
		return sm_Instance->m_MouseY;
	}

	static IntVector2 GetMousePosition()
	{
		return IntVector2(sm_Instance->m_MouseX, sm_Instance->m_MouseY);
	}

protected:

	//= Protected Functions ====================================

	virtual bool IsKeyPressed_Implementation(int Keycode) = 0;

protected:

	//= Protected Variables ====================================

	std::bitset<256>	m_InputStates;
	int					m_MouseX;
	int					m_MouseY;

private:

	//= Private Static Variables ===============================
	static Input*		sm_Instance;
};