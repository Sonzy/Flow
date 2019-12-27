#pragma once

#include "Flow/Core.h"
#include "Flow/Helper/Maths.h"

namespace Flow
{
	//TODO: Make buffered events instead of blocking
	enum class EventType
	{
		None = 0,
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,
		ApplicationTick, //TODO: Might be unused
		ApplicationUpdate,//TODO: Might be unused
		ApplicationRender,//TODO: Might be unused
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		Application		= BITMASK(0),
		Input			= BITMASK(1),
		Keyboard		= BITMASK(2),
		Mouse			= BITMASK(3),
		MouseButton		= BITMASK(4),
	};

	//Lazy method of defining these repeated function
#define EVENT_CLASS_TYPE(Type)  static EventType GetStaticType() { return EventType::##Type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #Type; }

		//Lazy method of defining the category flag returns
#define EVENT_CLASS_CATEGORY(Category) virtual int GetCategoryFlags() const override { return Category; }

	class FLOW_API Event
	{
		friend class EventDispatcher; //TODO
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		virtual std::string ToString() const;

		bool IsInCategory(EventCategory Category) const;
	protected:
		bool bHandled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFunction = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event);

		template<typename T>
		bool Dispatch(EventFunction<T> Function)
		{
			if (m_Event.GetEventType() != T::GetStaticType())
				return false;

			m_Event.bHandled = Function(*(T*)&m_Event); //bruh this casting lul
			return true;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}