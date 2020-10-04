#pragma once

//= Includes ==============================================

#include "Event.h"

//= Class Definitions ====================================

class FLOW_API MouseMovedEvent : public Event
{
public:
	//= Public Functions ===================

					MouseMovedEvent(int X, int Y);

	int				GetX() const;
	int				GetY() const;
	std::string		ToString() const override;

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(Event::Category::Mouse | Event::Category::UserInput)

private:
	//= Private Variables ===================

	int				m_MouseX;
	int				m_MouseY;
};

class FLOW_API MouseScrolledEvent : public Event
{
public:
	//= Public Functions ===================

					MouseScrolledEvent(float XOffset, float YOffset);
	float			GetXOffset() const;
	float			GetYOffset() const;
	std::string		ToString() const override;

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(Event::Category::Mouse | Event::Category::UserInput)

private:
	//= Protected Variables ===================

	float			m_XOffset;
	float			m_YOffset;
};

class FLOW_API MouseButtonEvent : public Event
{
public:
	//= Public Functions ===================

	int				GetMouseButton() const;

	EVENT_CLASS_CATEGORY(Event::Category::Mouse | Event::Category::UserInput | Event::Category::MouseButton)
protected:
	//= Protected Functions ===================

					MouseButtonEvent(int Button);

protected:
	//= Protected Variables ===================

	int				m_Button;
};

/* Mouse button event, 0 = Left, 1 = Right, 2 = Middle */
class FLOW_API MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	//= Public Functions ===================

					MouseButtonPressedEvent(int Button);
	std::string		ToString() const override;

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

class FLOW_API MouseButtonReleasedEvent : public MouseButtonEvent
{
public:

	//= Public Functions ===================

					MouseButtonReleasedEvent(int Button);
	std::string		ToString() const override;

	EVENT_CLASS_TYPE(MouseButtonReleased)
};