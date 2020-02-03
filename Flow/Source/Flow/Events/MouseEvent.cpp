#include "Flowpch.h"
#include "MouseEvent.h"
#include "Flow/Input/Input.h"

namespace Flow
{

	//= Mouse Moved ======================================================

	MouseMovedEvent::MouseMovedEvent(int X, int Y)
		: MouseX(X), MouseY(Y)
	{
		Input::OnMouseMoved(X, Y);
	}

	int Flow::MouseMovedEvent::GetX() const
	{
		return MouseX;
	}

	int Flow::MouseMovedEvent::GetY() const
	{
		return MouseY;
	}

	std::string Flow::MouseMovedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << MouseX << ", " << MouseY;
		return ss.str();
	}

	//= Mouse Scrolled ======================================================


	MouseScrolledEvent::MouseScrolledEvent(float XOffset, float YOffset)
		: XOffset(XOffset), YOffset(YOffset)
	{

	}

	float MouseScrolledEvent::GetXOffset() const
	{
		return XOffset;
	}

	float MouseScrolledEvent::GetYOffset() const
	{
		return YOffset;
	}

	std::string MouseScrolledEvent::ToString() const
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << XOffset << ", " << YOffset;
		return ss.str();
	}


	//= MouseButtonEvent ======================================


	int MouseButtonEvent::GetMouseButton() const
	{
		return Button;
	}

	MouseButtonEvent::MouseButtonEvent(int Button)
		: Button(Button)
	{
	}


	MouseButtonPressedEvent::MouseButtonPressedEvent(int Button)
		: MouseButtonEvent(Button)
	{
		switch (Button)
		{
		case 0:
			Input::OnKeyPressed(FLOW_MOUSE_LEFT);
			break;
		case 1:
			Input::OnKeyPressed(FLOW_MOUSE_RIGHT);
			break;
		case 2:
			Input::OnKeyPressed(FLOW_MOUSE_MIDDLE);
			break;
		}
	}

	std::string MouseButtonPressedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << Button;
		return ss.str();
	}

	MouseButtonReleasedEvent::MouseButtonReleasedEvent(int Button)
		: MouseButtonEvent(Button)
	{
		switch (Button)
		{
		case 0:
			Input::OnKeyReleased(FLOW_MOUSE_LEFT);
			break;
		case 1:
			Input::OnKeyReleased(FLOW_MOUSE_RIGHT);
			break;
		case 2:
			Input::OnKeyReleased(FLOW_MOUSE_MIDDLE);
			break;
		}
	}

	std::string MouseButtonReleasedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << Button;
		return ss.str();
	}

}