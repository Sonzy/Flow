#pragma once
#include "Event.h"

namespace Flow
{
	class FLOW_API KeyEvent : public Event
	{
	public:
		[[nodiscard]] int GetKeyCode() const;

		EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)
	protected:
		KeyEvent(int KeyCode);

	protected:
		int KeyCode_;
	};


	class FLOW_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int KeyCode, int RepeatCount);


		int GetRepeatCount() const;
		[[nodiscard]] std::string ToString() const override;

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int RepeatCount_;
	};

	class FLOW_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int KeyCode);


		[[nodiscard]] std::string ToString() const override;

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class FLOW_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int KeyCode)
			: KeyEvent(KeyCode)
		{}

		[[nodiscard]] std::string ToString() const override;

		EVENT_CLASS_TYPE(KeyTyped)
	};
}