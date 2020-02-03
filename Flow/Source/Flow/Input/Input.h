#pragma once
#include "Flow/Core.h"
#include <bitset>
#include "KeyCodes.h"
#include "Flow\Helper\Maths.h"

namespace Flow
{
	class FLOW_API Input
	{
	public:

		static bool IsKeyPressed(int Keycode)
		{
			return s_Instance->IsKeyPressed_Implementation(Keycode);
		}

		static bool IsMousePressed(int MouseButton)
		{
			return s_Instance->InputStates[MouseButton];
		}

		static void OnKeyPressed(int KeyCode)
		{
			s_Instance->InputStates[KeyCode] = true;
		}

		static void OnKeyReleased(int KeyCode)
		{
			s_Instance->InputStates[KeyCode] = false;
		}

		static void OnMouseMoved(int X, int Y)
		{
			s_Instance->MouseX = X;
			s_Instance->MouseY = Y;
		}

		static int GetMouseX()
		{
			return s_Instance->MouseX;
		}

		static int GetMouseY()
		{
			return s_Instance->MouseY;
		}

		static IntVector2D GetMousePosition()
		{
			return IntVector2D(s_Instance->MouseX, s_Instance->MouseY);
		}


	protected:
		virtual bool IsKeyPressed_Implementation(int Keycode) = 0;

		std::bitset<256> InputStates;

		int MouseX;
		int MouseY;
	private:


		static Input* s_Instance;
	};
}