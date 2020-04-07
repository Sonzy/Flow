#include "Flowpch.h"
#include "ApplicationEvent.h"

namespace Flow
{
	//= Window Events =========================

	WindowResizedEvent::WindowResizedEvent(unsigned int width, unsigned int height)
		: Width_(width), Height_(height)
	{
	}

	int WindowResizedEvent::GetWidth() const
	{
		return Width_;
	}

	int WindowResizedEvent::GetHeight() const
	{
		return Height_;
	}

	std::string WindowResizedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "WindowResizedEvent: " << Width_ << ", " << Height_;
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