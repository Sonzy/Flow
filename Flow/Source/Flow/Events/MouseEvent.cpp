#include "pch.h"
#include "MouseEvent.h"
#include "Flow/Input/Input.h"

//= Mouse Moved ======================================================

MouseMovedEvent::MouseMovedEvent(int X, int Y)
	: m_MouseX(X), m_MouseY(Y)
{
	Input::OnMouseMoved(X, Y);
}

int MouseMovedEvent::GetX() const
{
	return m_MouseX;
}

int MouseMovedEvent::GetY() const
{
	return m_MouseY;
}

std::string MouseMovedEvent::ToString() const
{
	std::stringstream ss;
	ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
	return ss.str();
}

//= Mouse Scrolled ======================================================


MouseScrolledEvent::MouseScrolledEvent(float XOffset, float YOffset)
	: m_XOffset(XOffset), m_YOffset(YOffset)
{

}

float MouseScrolledEvent::GetXOffset() const
{
	return m_XOffset;
}

float MouseScrolledEvent::GetYOffset() const
{
	return m_YOffset;
}

std::string MouseScrolledEvent::ToString() const
{
	std::stringstream ss;
	ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
	return ss.str();
}


//= MouseButtonEvent ======================================


int MouseButtonEvent::GetMouseButton() const
{
	return m_Button;
}

MouseButtonEvent::MouseButtonEvent(int Button)
	: m_Button(Button)
{
}


MouseButtonPressedEvent::MouseButtonPressedEvent(int Button)
	: MouseButtonEvent(Button)
{
	switch (Button)
	{
	case 0:
		Input::OnKeyPressed(MOUSE_LEFT);
		break;
	case 1:
		Input::OnKeyPressed(MOUSE_RIGHT);
		break;
	case 2:
		Input::OnKeyPressed(MOUSE_MIDDLE);
		break;
	}
}

std::string MouseButtonPressedEvent::ToString() const
{
	std::stringstream ss;
	ss << "MouseButtonPressedEvent: " << m_Button;
	return ss.str();
}

MouseButtonReleasedEvent::MouseButtonReleasedEvent(int Button)
	: MouseButtonEvent(Button)
{
	switch (Button)
	{
	case 0:
		Input::OnKeyReleased(MOUSE_LEFT);
		break;
	case 1:
		Input::OnKeyReleased(MOUSE_RIGHT);
		break;
	case 2:
		Input::OnKeyReleased(MOUSE_MIDDLE);
		break;
	}
}

std::string MouseButtonReleasedEvent::ToString() const
{
	std::stringstream ss;
	ss << "MouseButtonReleasedEvent: " << m_Button;
	return ss.str();
}