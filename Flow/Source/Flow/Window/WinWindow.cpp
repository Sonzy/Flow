#include "Flowpch.h"
#include "WinWindow.h"
#include "Flow/Core.h"

#include "Flow/Events/ApplicationEvent.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow/Events/KeyEvent.h"

#include "Flow/Rendering/Renderer.h"

namespace Flow
{
	Window* Window::Create(const WindowProperties& Properties)
	{
		return new WinWindow(Properties);
	}

	WinWindow::WinWindow(const WindowProperties& Properties)
	{
		Initialise(Properties);

		RenderCommand::InitialiseDX11(GetWindowHandle(), Properties.Width, Properties.Height);
	}

	WinWindow::~WinWindow()
	{
		Shutdown();
	}

	void WinWindow::Initialise(const WindowProperties& Properties)
	{
		bool result;

		RECT WindowRegion;
		WindowRegion.left = 100;
		WindowRegion.right = Properties.Width + WindowRegion.left;
		WindowRegion.top = 100;
		WindowRegion.bottom = Properties.Height + WindowRegion.top;

		DWORD WindowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

		result = AdjustWindowRect(&WindowRegion, WindowStyle, FALSE);
		FLOW_ASSERT(result, "WinWindow::Initialise: Failed to adjust the window rect");

		WindowHandle = CreateWindowEx(
			0,
			WindowClass::GetName(),
			Properties.Title.c_str(),
			WindowStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			WindowRegion.right - WindowRegion.left,
			WindowRegion.bottom - WindowRegion.top,
			nullptr,
			nullptr,
			WindowClass::GetInstanceHandle(),
			this
		);

		ShowWindow(WindowHandle, SW_SHOW);

		Props = Properties;

		m_WindowData.Title = Props.Title;
		m_WindowData.Width = Props.Width;
		m_WindowData.Height = Props.Height;

		RenderCommand::SetClearColour(0.1f, 0.1f, 0.1f, 1.0f);
	}

	void WinWindow::Shutdown()
	{
		DestroyWindow(WindowHandle);
	}

	void WinWindow::PreUpdate()
	{
		RenderCommand::BeginFrame();
	}

	void WinWindow::OnUpdate()
	{
		ProcessWindowsMessages();
	}

	void WinWindow::PostUpdate()
	{
		RenderCommand::EndFrame();
	}

	unsigned int WinWindow::GetWidth() const
	{
		return Props.Width;
	}

	unsigned int WinWindow::GetHeight() const
	{
		return Props.Height;
	}

	void WinWindow::SetEventCallback(const EventCallbackFunction& Callback)
	{
		m_WindowData.EventCallback = Callback;
	}

	void WinWindow::EnableVSync(bool bEnabled)
	{
		FLOW_ENGINE_LOG("TODO: WinWindow::EnableVSync");
	}

	bool WinWindow::IsVSyncEnabled() const
	{
		FLOW_ENGINE_LOG("TODO: WinWindow::IsVSyncEnabled");
		return false;
	}

	LRESULT __stdcall WinWindow::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg != WM_NCCREATE)
			return DefWindowProc(hWnd, msg, wParam, lParam);

		//Get window pointer from creation data
		const CREATESTRUCTW* const creationStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WinWindow* const WindowPtr = static_cast<WinWindow*>(creationStruct->lpCreateParams);

		//Store pointer to window class in the WinApi user data
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(WindowPtr));
		//Set the windows message handling to use my function
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WinWindow::HandleMsgThunk));

		return WindowPtr->HandleMessages(hWnd, msg, wParam, lParam);
	}

	LRESULT __stdcall WinWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// Pass the message to the window message handler
		WinWindow* const window = reinterpret_cast<WinWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return window->HandleMessages(hWnd, msg, wParam, lParam);
	}

	LRESULT WinWindow::HandleMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Event* e = nullptr;
		bool bHandled = true;
		switch (msg)
		{
			case WM_CLOSE:
			{
				WindowClosedEvent Event;
				e = &Event;
				break;
			}

			//= Mouse Events =====================

			case WM_MOUSEMOVE:
			{
				const POINTS points = MAKEPOINTS(lParam);
				MouseMovedEvent Event(points.x, points.y);
				e = &Event;
				break;
			}
			case WM_LBUTTONDOWN:
			{
				MouseButtonPressedEvent Event(0);
				e = &Event;
				break;
			}
			case WM_LBUTTONUP:
			{
				MouseButtonReleasedEvent Event(0);
				e = &Event;
				break;
			}
			case WM_RBUTTONDOWN:
			{
				MouseButtonPressedEvent Event(1);
				e = &Event;
				break;
			}
			case WM_RBUTTONUP:
			{
				MouseButtonReleasedEvent Event(1);
				e = &Event;
				break;
			}
			case WM_MBUTTONDOWN:
			{
				MouseButtonPressedEvent Event(2);
				e = &Event;
				break;
			}
			case WM_MBUTTONUP:
			{
				MouseButtonReleasedEvent Event(2);
				e = &Event;
				break;
			}
			case WM_MOUSEWHEEL:
			{
				constexpr int Division = 120;
				float yDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				MouseScrolledEvent Event(0.0f, yDelta / Division);
				e = &Event;
				break;
			}

			//= Key Events ===============================

			//TODO: Left/Right Key specification https://stackoverflow.com/questions/15966642/how-do-you-tell-lshift-apart-from-rshift-in-wm-keydown-events
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				KeyPressedEvent Event(wParam, 0);
				e = &Event;
				break;
			}
			case WM_KEYUP:
			{
				KeyReleasedEvent Event(wParam);
				e = &Event;
				break;
			}
			case WM_CHAR:
			{
				KeyTypedEvent Event(static_cast<unsigned char>(wParam));
				e = &Event;
				break;
			}


		default:
				bHandled = false;
		}


		//If we have a handled event, call the callback
		if (e)
			m_WindowData.EventCallback(*e);
		else if(bHandled)
		{
			FLOW_ENGINE_ERROR("WinWindow::HandleMessages: Event was nullptr");
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}


	std::optional<int> WinWindow::ProcessWindowsMessages()
	{
		MSG Message;
		while (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				return (int)Message.wParam;

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		return {};
	}

	HWND& WinWindow::GetWindowHandle()
	{
		return WindowHandle;
	}

	//= Window Class ===========================

	WindowClass WindowClass::WindowClassInstance;

	const char* WindowClass::GetName()
	{
		return ClassName;
	}
	HINSTANCE WindowClass::GetInstanceHandle()
	{
		return WindowClassInstance.InstanceHandle;
	}
	WindowClass::WindowClass()
	{
		WNDCLASSEX windowsClass = { 0 };
		windowsClass.cbSize = sizeof(windowsClass);
		windowsClass.style = CS_OWNDC;
		windowsClass.lpfnWndProc = WinWindow::HandleMessageSetup; //Link to the function that will be handling windows messages
		windowsClass.cbClsExtra = 0;
		windowsClass.cbWndExtra = 0;
		windowsClass.hInstance = GetInstanceHandle();
		windowsClass.hIcon = nullptr; // static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0)); //Used to set up custom icon
		windowsClass.hIconSm = nullptr; //static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));; //Same as before but small one
		windowsClass.lpszClassName = ClassName;

		RegisterClassEx(&windowsClass);
	}
	WindowClass::~WindowClass()
	{
		UnregisterClass(ClassName, GetInstanceHandle());
	}
}