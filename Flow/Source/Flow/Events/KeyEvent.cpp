#include "Flowpch.h"
#include "KeyEvent.h"
#include "Flow/Input/Input.h"

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

	KeyPressedEvent::KeyPressedEvent(int KeyCode, int RepeatCount)
		: KeyEvent(KeyCode), RepeatCount(RepeatCount)
	{
		Input::OnKeyPressed(KeyCode);
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
	KeyReleasedEvent::KeyReleasedEvent(int KeyCode)
		: KeyEvent(KeyCode)
	{
		Input::OnKeyReleased(KeyCode);
	}

	std::string KeyReleasedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << KeyCode;
		return ss.str();
	}
	std::string KeyTypedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << KeyCode;
		return ss.str();
	}
}