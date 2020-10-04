#pragma once

//= Includes ==================================================

#include "Event.h"

//= Class Definitions =========================================

class FLOW_API WindowResizedEvent : public Event
{
public:

	//= Public Functions ==========================
	WindowResizedEvent(unsigned int width, unsigned int height);

	int GetWidth() const;
	int GetHeight() const;

	std::string ToString() const override;

	EVENT_CLASS_TYPE(WindowResized)
	EVENT_CLASS_CATEGORY(Event::Category::App)
private:

	//= Private Variables =========================

	unsigned int m_Width;
	unsigned int m_Height;
};

class FLOW_API WindowClosedEvent : public Event
{
public:

	//= Public Functions =========================

	WindowClosedEvent();

	EVENT_CLASS_TYPE(WindowClosed)
	EVENT_CLASS_CATEGORY(Event::Category::App)
};

class FLOW_API ApplicationTickEvent : public Event
{
public:

	//= Public Functions =========================

	ApplicationTickEvent();

	EVENT_CLASS_TYPE(ApplicationTick)
	EVENT_CLASS_CATEGORY(Event::Category::App)
};

class FLOW_API ApplicationUpdateEvent : public Event
{
public:

	//= Public Functions =========================

	ApplicationUpdateEvent();

	EVENT_CLASS_TYPE(ApplicationUpdate)
	EVENT_CLASS_CATEGORY(Event::Category::App)
};

class FLOW_API ApplicationRenderEvent : public Event
{
public:

	//= Public Functions =========================

	ApplicationRenderEvent();

	EVENT_CLASS_TYPE(ApplicationRender)
	EVENT_CLASS_CATEGORY(Event::Category::App)
};