#include "Flowpch.h"
#include "KeyEvent.h"
#include "Flow/Input/Input.h"

namespace Flow
{
	KeyEvent::KeyEvent(int KeyCode)
		: KeyCode_(KeyCode)
	{
	
	}

	int KeyEvent::GetKeyCode() const
	{
		return KeyCode_;
	}

	KeyPressedEvent::KeyPressedEvent(int KeyCode, int RepeatCount)
		: KeyEvent(KeyCode), RepeatCount_(RepeatCount)
	{
		Input::OnKeyPressed(KeyCode);
	}

	int KeyPressedEvent::GetRepeatCount() const
	{
		return RepeatCount_;
	}

	std::string KeyPressedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << KeyCode_ << " (" << RepeatCount_ << " repeats)";
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
		ss << "KeyReleasedEvent: " << KeyCode_;
		return ss.str();
	}
	std::string KeyTypedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << KeyCode_;
		return ss.str();
	}
}