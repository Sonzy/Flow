#pragma once

//= Includes ========================================

#include "pch.h"
#include "Core.h"
#include "Events/Event.h"
#include "Rendering/Renderer.h"

#ifdef FLOW_PLATFORM_WINDOWS
	#include "Rendering/DX11/DX11RenderAPI.h"
#endif

//= Class Definitions ==================================

class FLOW_API Window
{
public:

	//= Public Typedefs ===============================================

	using EventCallbackFunction = std::function<void(Event&)>;

public:

	//= Public Structs ================================================

	struct Properties
	{
		Properties(const std::string& WindowTitle = "Flow Engine",
			unsigned int Width = 1280, unsigned int Height = 720)
			: m_Title(WindowTitle)
			, m_Width(Width)
			, m_Height(Height)
		{

		}

		std::string		m_Title;
		unsigned int	m_Width;
		unsigned int	m_Height;
	};

public:

	//= Public Functions ===============================================

	virtual					~Window() {};
	static Window*			Create(const Window::Properties& Properties, bool MainWindow = true);

	virtual void			PreUpdate() = 0;
	virtual void			OnUpdate() = 0;
	virtual void			PostUpdate() = 0;
	virtual void			Shutdown() = 0;

	virtual unsigned int	GetWidth() const = 0;
	virtual unsigned int	GetHeight() const = 0;

	virtual void			SetEventCallback(const EventCallbackFunction& Callback) = 0;
	virtual void			EnableVSync(bool bEnabled) = 0;
	virtual bool			IsVSyncEnabled() const = 0;

	virtual void			Resize(int Width, int Height) = 0;

protected:

	//= Protected Variables ============================================

	Window::Properties		m_Props;
	bool					m_MainWindow;
};