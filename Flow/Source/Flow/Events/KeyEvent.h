#pragma once
#include "Event.h"

namespace Flow
{
	class FLOW_API KeyEvent : public Event
	{
	public:
		int GetKeyCode() const;

		EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)
	protected:
		KeyEvent(int KeyCode);

	protected:
		int KeyCode;
	};


	class FLOW_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int KeyCode, int RepeatCount);


		int GetRepeatCount() const;
		std::string ToString() const override;

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int RepeatCount;
	};

	class FLOW_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int KeyCode);


		std::string ToString() const override;

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class FLOW_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int KeyCode)
			: KeyEvent(KeyCode)
		{

		}

		std::string ToString() const override;

		EVENT_CLASS_TYPE(KeyTyped)
	};
}