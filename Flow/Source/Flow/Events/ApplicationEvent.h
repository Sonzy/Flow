#pragma once
#include "Event.h"

namespace Flow
{
	class FLOW_API WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(unsigned int width, unsigned int height);

		int GetWidth() const;
		int GetHeight() const;

		std::string ToString() const override;

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategory::App)
	private:
		unsigned int Width;
		unsigned int Height;
	};

	class FLOW_API WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent();

		EVENT_CLASS_TYPE(WindowClose)
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