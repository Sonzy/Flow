#pragma once
#include "Event.h"

namespace Flow
{
	class FLOW_API WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(unsigned int width, unsigned int height);

		[[nodiscard]] int GetWidth() const;
		[[nodiscard]] int GetHeight() const;

		std::string ToString() const override;

		EVENT_CLASS_TYPE(WindowResized)
		EVENT_CLASS_CATEGORY(EventCategory::App)
	private:
		unsigned int Width_;
		unsigned int Height_;
	};

	class FLOW_API WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent();

		EVENT_CLASS_TYPE(WindowClosed)
		EVENT_CLASS_CATEGORY(EventCategory::App)
	};

	class FLOW_API ApplicationTickEvent : public Event
	{
	public:
		ApplicationTickEvent();

		EVENT_CLASS_TYPE(ApplicationTick)
		EVENT_CLASS_CATEGORY(EventCategory::App)
	};

	class FLOW_API ApplicationUpdateEvent : public Event
	{
	public:
		ApplicationUpdateEvent();

		EVENT_CLASS_TYPE(ApplicationUpdate)
		EVENT_CLASS_CATEGORY(EventCategory::App)
	};

	class FLOW_API ApplicationRenderEvent : public Event
	{
	public:
		ApplicationRenderEvent();

		EVENT_CLASS_TYPE(ApplicationRender)
		EVENT_CLASS_CATEGORY(EventCategory::App)
	};
}