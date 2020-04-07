#include "Flowpch.h"
#include "Event.h"

namespace Flow
{
	std::string Event::ToString() const
	{
		return GetName();
	}

	bool Event::IsInCategory(EventCategory Category) const
	{
		return GetCategoryFlags() & Category;
	}

	EventDispatcher::EventDispatcher(Event& event)
		: Event_(event)
	{
	}
}


