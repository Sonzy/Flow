#pragma once

//= Includes =========================================

#include <functional>
#include "Core.h"

//= Macro Definitions ================================

#define CONSUMES_INPUT(DispatchInputFunction, InputFunction) if(DispatchInputFunction){ bool Consumes = InputFunction; if(Consumes) return Consumes;}

//Lazy method of defining these repeated function
#define EVENT_CLASS_TYPE(TypeName)  static Event::Type GetStaticType() { return Event::Type::##TypeName; }\
								virtual Event::Type GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #TypeName; }

//Lazy method of defining the category flag returns
#define EVENT_CLASS_CATEGORY(Category) virtual int GetCategoryFlags() const override { return static_cast<int>(Category); }

//= Class Definitions ================================

class FLOW_API Event
{
	friend class EventDispatcher;
public:

	//= Public Enums =========================

	//TODO: Make buffered events instead of blocking
	enum class Type
	{
		None = 0,
		WindowClosed,
		WindowResized,
		WindowMaximised,
		WindowMinimized,
		WindowRestored,
		WindowFocused,
		WindowLostFocus,
		WindowMoved,
		ApplicationTick, //TODO: Might be unused
		ApplicationUpdate,//TODO: Might be unused
		ApplicationRender,//TODO: Might be unused
		KeyPressed,
		KeyReleased,
		KeyTyped,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	enum Category
	{
		None = 0,
		App = 1,
		UserInput = 2,
		Keyboard = 4,
		Mouse = 8,
		MouseButton = 16,
	};

public:
	
	//= Public Functions =======================

	virtual Event::Type GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetCategoryFlags() const = 0;
	virtual std::string ToString() const;

	bool IsInCategory(Event::Category Category) const;

	// Whether this event has already been handled
	bool m_Handled = false;
};

class EventDispatcher
{
	//= Type Definitions ========================
	template<typename T>
	using EventFunction = std::function<bool(T&)>;
public:

	//= Public Functions ========================

	EventDispatcher(Event& event);


	//= Public Template Functions ==============

	template<typename T>
	bool Dispatch(EventFunction<T> Function)
	{
		if (m_Event.GetEventType() != T::GetStaticType())
			return false;

		m_Event.m_Handled = Function(*(T*)&m_Event);
		return true;
	}

	template<typename T>
	static bool IsMatchingFunction(Event& event)
	{
		return event.GetEventType() == T::GetStaticType();
	}

private:
	Event& m_Event;
};

//= Inline Functions ==============================================

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
	return os << e.ToString();
}