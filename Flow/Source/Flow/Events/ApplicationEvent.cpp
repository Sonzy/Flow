#include "Flowpch.h"
#include "ApplicationEvent.h"

//= Window Events =========================

WindowResizedEvent::WindowResizedEvent(unsigned int width, unsigned int height)
	: _Width(width), _Height(height)
{
}

int WindowResizedEvent::GetWidth() const
{
	return _Width;
}

int WindowResizedEvent::GetHeight() const
{
	return _Height;
}

std::string WindowResizedEvent::ToString() const
{
	std::stringstream ss;
	ss << "WindowResizedEvent: " << _Width << ", " << _Height;
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

WindowClosedEvent::WindowClosedEvent()
{
}