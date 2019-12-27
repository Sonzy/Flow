#pragma once
#include "Flowpch.h"

#include "Flow/Core.h"
#include "Flow/Events/Event.h"

namespace Flow
{
	struct WindowProperties
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProperties(const std::string& WindowTitle = "Flow Engine",
			unsigned int Width = 1280, unsigned int Height = 720)
			: Title(WindowTitle), Width(Width), Height(Height)
		{

		}
	};

	class FLOW_API Window
	{
	public:
		using EventCallbackFunction = std::function<void(Event&)>;

		virtual ~Window() {};

		static Window* Create(const WindowProperties& Properties);

		virtual void OnUpdate() = 0;
		
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunction& Callback) = 0;
		virtual void EnableVSync(bool bEnabled) = 0;
		virtual bool IsVSyncEnabled() const = 0;

	protected:
		WindowProperties Props;
	};
}