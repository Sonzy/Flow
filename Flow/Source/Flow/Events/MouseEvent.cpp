#include "Flowpch.h"
#include "MouseEvent.h"
#include "Flow/Input/Input.h"

//= Mouse Moved ======================================================

MouseMovedEvent::MouseMovedEvent(int X, int Y)
	: _MouseX(X), _MouseY(Y)
{
	Input::OnMouseMoved(X, Y);
}

int MouseMovedEvent::GetX() const
{
	return _MouseX;
}

int MouseMovedEvent::GetY() const
{
	return _MouseY;
}

std::string MouseMovedEvent::ToString() const
{
	std::stringstream ss;
	ss << "MouseMovedEvent: " << _MouseX << ", " << _MouseY;
	return ss.str();
}

//= Mouse Scrolled ======================================================


MouseScrolledEvent::MouseScrolledEvent(float XOffset, float YOffset)
	: _XOffset(XOffset), _YOffset(YOffset)
{

}

float MouseScrolledEvent::GetXOffset() const
{
	return _XOffset;
}

float MouseScrolledEvent::GetYOffset() const
{
	return _YOffset;
}

std::string MouseScrolledEvent::ToString() const
{
	std::stringstream ss;
	ss << "MouseScrolledEvent: " << _XOffset << ", " << _YOffset;
	return ss.str();
}


//= MouseButtonEvent ======================================


int MouseButtonEvent::GetMouseButton() const
{
	return _Button;
}

MouseButtonEvent::MouseButtonEvent(int Button)
	: _Button(Button)
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
	ss << "MouseButtonPressedEvent: " << _Button;
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
	ss << "MouseButtonReleasedEvent: " << _Button;
	return ss.str();
}