#pragma once
#include "Flowpch.h"

#include "Flow/Core.h"
#include "Flow/Events/Event.h"
#include "Flow/Rendering/Renderer.h"

#ifdef FLOW_PLATFORM_WINDOWS
#include "Flow/Rendering/DX11/DX11Renderer.h"
#endif

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

		virtual void PreUpdate() = 0;
		virtual void OnUpdate() = 0;
		virtual void PostUpdate() = 0;
		
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunction& Callback) = 0;
		virtual void EnableVSync(bool bEnabled) = 0;
		virtual bool IsVSyncEnabled() const = 0;


		template<typename T>
		T* GetRenderer()
		{
			//TODO: Ensure valid renderer type
			return dynamic_cast<T*>(CurrentRenderer.get());
		}

	protected:
		WindowProperties Props;

		void CreateRenderer(RenderAPI API, HWND WindowHandle, int Height, int Width)
		{
			switch (API)
			{
			case Flow::RenderAPI::DirectX11:
				CurrentRenderer = std::make_unique<DX11Renderer>(WindowHandle, Height, Width);
				break;
			default:
				FLOW_ENGINE_ERROR("Window::CreateRenderer: Selected RenderAPI is currently not supported.");
				break;
			}
		}

		std::unique_ptr<Renderer> CurrentRenderer;
	};
}