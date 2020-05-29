#include "Flowpch.h"
#include "KeyEvent.h"
#include "Flow/Input/Input.h"

KeyEvent::KeyEvent(int KeyCode)
	: _KeyCode(KeyCode)
{

}

int KeyEvent::GetKeyCode() const
{
	return _KeyCode;
}

KeyPressedEvent::KeyPressedEvent(int KeyCode, int RepeatCount)
	: KeyEvent(KeyCode), _RepeatCount(RepeatCount)
{
	Input::OnKeyPressed(KeyCode);
}

int KeyPressedEvent::GetRepeatCount() const
{
	return _RepeatCount;
}

std::string KeyPressedEvent::ToString() const
{
	std::stringstream ss;
	ss << "KeyPressedEvent: " << _KeyCode << " (" << _RepeatCount << " repeats)";
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
	ss << "KeyReleasedEvent: " << _KeyCode;
	return ss.str();
}
std::string KeyTypedEvent::ToString() const
{
	std::stringstream ss;
	ss << "KeyTypedEvent: " << _KeyCode;
	return ss.str();
}