#include "pch.h"
#include "KeyEvent.h"
#include "Flow/Input/Input.h"

KeyEvent::KeyEvent(int KeyCode)
	: m_KeyCode(KeyCode)
{

}

int KeyEvent::GetKeyCode() const
{
	return m_KeyCode;
}

KeyPressedEvent::KeyPressedEvent(int KeyCode, int RepeatCount)
	: KeyEvent(KeyCode), m_RepeatCount(RepeatCount)
{
	Input::OnKeyPressed(KeyCode);
}

int KeyPressedEvent::GetRepeatCount() const
{
	return m_RepeatCount;
}

std::string KeyPressedEvent::ToString() const
{
	std::stringstream ss;
	ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
	return ss.str();
}
KeyReleasedEvent::KeyReleasedEvent(int KeyCode)
	: KeyEvent(KeyCode)
{
	Input::OnKeyReleased(KeyCode);
}

std::string KeyReleasedEvent::ToString() const
{
	std::stringstream ss;
	ss << "KeyReleasedEvent: " << m_KeyCode;
	return ss.str();
}
std::string KeyTypedEvent::ToString() const
{
	std::stringstream ss;
	ss << "KeyTypedEvent: " << m_KeyCode;
	return ss.str();
}