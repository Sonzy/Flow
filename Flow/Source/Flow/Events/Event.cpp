#include "Flowpch.h"
#include "Event.h"

std::string Event::ToString() const
{
	return GetName();
}

bool Event::IsInCategory(EventCategory Category) const
{
	return GetCategoryFlags() & Category;
}

EventDispatcher::EventDispatcher(Event& event)
	: _Event(event)
{
}

