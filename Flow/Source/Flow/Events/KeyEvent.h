#pragma once
#include "Event.h"

class FLOW_API KeyEvent : public Event
{
public:
	[[nodiscard]] int GetKeyCode() const;

	EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)
protected:
	KeyEvent(int KeyCode);

protected:
	int _KeyCode;
};


class FLOW_API KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int KeyCode, int RepeatCount);


	int GetRepeatCount() const;
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(KeyPressed)
private:
	int _RepeatCount;
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