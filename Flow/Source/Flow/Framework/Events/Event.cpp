#include "pch.h"
#include "Event.h"

std::string Event::ToString() const
{
	return GetName();
}

bool Event::IsInCategory(Event::Category Category) const
{
	return GetCategoryFlags() & (int)Category; //TODO: Check if I wiffed changing to enum class
}

EventDispatcher::EventDispatcher(Event& event)
	: m_Event(event)
{
}

