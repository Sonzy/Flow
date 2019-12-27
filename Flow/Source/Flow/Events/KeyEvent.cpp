#include "KeyEvent.h"
namespace Flow
{
	KeyEvent::KeyEvent(int KeyCode)
		: KeyCode(KeyCode)
	{

	}

	int KeyEvent::GetKeyCode() const
	{
		return KeyCode;
	}

	int KeyPressedEvent::GetRepeatCount() const
	{
		return RepeatCount;
	}

	std::string KeyPressedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << KeyCode << " (" << RepeatCount << " repeats)";
		return ss.str();
	}
	std::string KeyReleasedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << KeyCode;
		return ss.str();
	}
}