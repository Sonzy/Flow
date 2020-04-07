#include "Flowpch.h"
#include "MouseEvent.h"
#include "Flow/Input/Input.h"

namespace Flow
{

	//= Mouse Moved ======================================================

	MouseMovedEvent::MouseMovedEvent(int X, int Y)
		: MouseX_(X), MouseY_(Y)
	{
		Input::OnMouseMoved(X, Y);
	}

	int Flow::MouseMovedEvent::GetX() const
	{
		return MouseX_;
	}

	int Flow::MouseMovedEvent::GetY() const
	{
		return MouseY_;
	}

	std::string Flow::MouseMovedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << MouseX_ << ", " << MouseY_;
		return ss.str();
	}

	//= Mouse Scrolled ======================================================


	MouseScrolledEvent::MouseScrolledEvent(float XOffset, float YOffset)
		: XOffset_(XOffset), YOffset_(YOffset)
	{

	}

	float MouseScrolledEvent::GetXOffset() const
	{
		return XOffset_;
	}

	float MouseScrolledEvent::GetYOffset() const
	{
		return YOffset_;
	}

	std::string MouseScrolledEvent::ToString() const
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << XOffset_ << ", " << YOffset_;
		return ss.str();
	}


	//= MouseButtonEvent ======================================


	int MouseButtonEvent::GetMouseButton() const
	{
		return Button_;
	}

	MouseButtonEvent::MouseButtonEvent(int Button)
		: Button_(Button)
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
		ss << "MouseButtonPressedEvent: " << Button_;
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
		ss << "MouseButtonReleasedEvent: " << Button_;
		return ss.str();
	}

}