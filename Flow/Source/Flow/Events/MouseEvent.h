#pragma once
#include "Event.h"

namespace Flow
{
	class FLOW_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int X, int Y);

		int GetX() const;
		int GetY() const;

		std::string ToString() const override;

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)
	private:
		int MouseX_;
		int MouseY_;
	};

	class FLOW_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float XOffset, float YOffset);
		
		float GetXOffset() const;
		float GetYOffset() const;

		std::string ToString() const override;

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

	private:
		float XOffset_;
		float YOffset_;
	};

	class FLOW_API MouseButtonEvent : public Event
	{
	public:
		int GetMouseButton() const;

		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input | EventCategory::MouseButton)
	protected:
		MouseButtonEvent(int Button);

		int Button_;
	};

	/* Mouse button event, 0 = Left, 1 = Right, 2 = Middle */
	class FLOW_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int Button);

		std::string ToString() const override;

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class FLOW_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int Button);

		std::string ToString() const override;

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}