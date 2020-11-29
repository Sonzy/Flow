#include "Flowpch.h"
#include "ApplicationEvent.h"

//= Window Events =========================

WindowResizedEvent::WindowResizedEvent(unsigned int width, unsigned int height)
	: m_Width(width), m_Height(height)
{
}

int WindowResizedEvent::GetWidth() const
{
	return m_Width;
}

int WindowResizedEvent::GetHeight() const
{
	return m_Height;
}

std::string WindowResizedEvent::ToString() const
{
	std::stringstream ss;
	ss << "WindowResizedEvent: " << m_Width << ", " << m_Height;
	return ss.str();
}

WindowClosedEvent::WindowClosedEvent()
{
}

std::string WindowMinimizedEvent::ToString() const
{
	std::stringstream ss;
	ss << "WindowMinimizedEvent";
	return ss.str();
}

std::string WindowRestoredEvent::ToString() const
{
	std::stringstream ss;
	ss << "WindowRestoredEvent";
	return ss.str();
}

//= Application Events =========================


ApplicationTickEvent::ApplicationTickEvent()
{
}

ApplicationUpdateEvent::ApplicationUpdateEvent()
{
}

ApplicationRenderEvent::ApplicationRenderEvent()
{
}