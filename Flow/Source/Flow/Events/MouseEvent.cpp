#include "Flowpch.h"
#include "MouseEvent.h"

namespace Flow
{

	//= Mouse Moved ======================================================

	MouseMovedEvent::MouseMovedEvent(int X, int Y)
		: MouseX(X), MouseY(Y)
	{

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
	}

	std::string MouseButtonReleasedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << Button;
		return ss.str();
	}

}