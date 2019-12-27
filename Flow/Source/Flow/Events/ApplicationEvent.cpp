#include "ApplicationEvent.h"

namespace Flow
{
	//= Window Events =========================

	WindowResizedEvent::WindowResizedEvent(unsigned int width, unsigned int height)
		: Width(width), Height(height)
	{
	}

	int WindowResizedEvent::GetWidth() const
	{
		return Width;
	}

	int WindowResizedEvent::GetHeight() const
	{
		return Height;
	}

	std::string WindowResizedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "WindowResizedEvent: " << Width << ", " << Height;
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

}