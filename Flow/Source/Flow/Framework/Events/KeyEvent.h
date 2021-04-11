#pragma once

//= Includes =================================================

#include "Event.h"

//= Class Declaration ========================================

class FLOW_API KeyEvent : public Event
{
public:

	//= Public Functions =========================

	int				GetKeyCode() const;
	EVENT_CLASS_CATEGORY(Event::Category::Keyboard | Event::Category::UserInput)

protected:

	//= Protected Functions ======================

					KeyEvent(int KeyCode);
protected:

	//= Protected Variables ======================

	int				m_KeyCode;
};


class FLOW_API KeyPressedEvent : public KeyEvent
{
public:
	//= Public Functions =========================

						KeyPressedEvent(int KeyCode, int RepeatCount);
	int					GetRepeatCount() const;
	std::string			ToString() const override;
	EVENT_CLASS_TYPE(KeyPressed)


private:
	//= Private Variables ======================

	int m_RepeatCount;
};

class FLOW_API KeyReleasedEvent : public KeyEvent
{
public:

	//= Public Functions =========================

						KeyReleasedEvent(int KeyCode);
	std::string			ToString() const override;
	EVENT_CLASS_TYPE(KeyReleased)
};

class FLOW_API KeyTypedEvent : public KeyEvent
{
public:

	//= Public Functions =========================

	KeyTypedEvent(int KeyCode)
		: KeyEvent(KeyCode)
	{}
	std::string			ToString() const override;
	EVENT_CLASS_TYPE(KeyTyped)
};