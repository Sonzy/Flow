#include "Flowpch.h"
#include "WinWindow.h"
#include "Flow/Core.h"

#include "Flow/Events/ApplicationEvent.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow/Events/KeyEvent.h"

namespace Flow
{
	Window* Window::Create(const WindowProperties& Properties)
	{
		return new WinWindow(Properties);
	}

	WinWindow::WinWindow(const WindowProperties& Properties)
	{
		Initialise(Properties);
		DX11Renderer = std::make_unique<GraphicsDX11>(WindowHandle, Properties.Width, Properties.Height);
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

		DWORD WindowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

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

		//TODO: Initialise Window UI

		Props = Properties;

		m_WindowData.Title = Props.Title;
		m_WindowData.Width = Props.Width;
		m_WindowData.Height = Props.Height;
	}

	void WinWindow::Shutdown()
	{
		DestroyWindow(WindowHandle);
	}

	void WinWindow::PreUpdate()
	{
		DX11Renderer->ClearWindow(0.7f, 0.7f, 0.7f, 1.0f);
	}

	void WinWindow::OnUpdate()
	{
		ProcessWindowsMessages();
	}

	void WinWindow::PostUpdate()
	{
		DX11Renderer->EndFrame();
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


		////if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		////	return true;
		//
		////const auto GuiIO = ImGui::GetIO();
		//
		//switch (msg)
		//{
		//	//Window Messages================================
		//case WM_CLOSE:
		//	PostQuitMessage(0); //Sends a quit message with exit code param
		//	return 0;
		//case WM_KILLFOCUS: // When we lose focus, reset the keyboard state
		//	//GetKeyboard().ClearState();
		//case WM_ACTIVATE:
		//
		//	//if (bCursorEnabled)
		//	//	break;
		//	//
		//	//if (wParam & WA_ACTIVE || wParam & WA_CLICKACTIVE)
		//	//{
		//	//	ConfineCursor(true);
		//	//	HideCursor();
		//	//}
		//	//else
		//	//{
		//	//	ConfineCursor(false);
		//	//	ShowCursor();
		//	//}
		//	//
		//	break;
		//	//Key Messages===================================
		//case WM_SYSKEYDOWN:
		//case WM_KEYDOWN: //Not Case Sensitive
		//
		//	////If the GUI wants to capture the keyboard, let it over the app
		//	//if (GuiIO.WantCaptureKeyboard)
		//	//	break;
		//	//
		//	//if (!(lParam & 0x40000000) || GetKeyboard().IsAutorepeatEnabled())
		//	//	GetKeyboard().OnKeyPressed(static_cast<unsigned char>(wParam));
		//	//break;
		//case WM_SYSKEYUP:
		//case WM_KEYUP:
		//
		//	////If the GUI wants to capture the keyboard, let it over the app
		//	//if (GuiIO.WantCaptureKeyboard)
		//	//	break;
		//	//
		//	//GetKeyboard().OnKeyReleased(static_cast<unsigned char>(wParam));
		//	//break;
		//case WM_CHAR: // Case sensitive
		//
		//	// //If the GUI wants to capture the Keyboad, let it over the app
		//	//if (GuiIO.WantCaptureKeyboard)
		//	//	break;
		//	//
		//	//GetKeyboard().OnChar(static_cast<unsigned char>(wParam));
		//	//break;
		//	//GetMouse() Messages==================================
		//case WM_MOUSEMOVE:
		//{
		//	//If the GUI wants to capture the Keyboad, let it over the app
		//	//if (GuiIO.WantCaptureMouse)
		//	//	break;
		//	//
		//	//const POINTS points = MAKEPOINTS(lParam);
		//	////Check if we are in the window
		//	//if (points.x >= 0 && points.x < Width && points.y >= 0 && points.y < Height)
		//	//{
		//	//	GetMouse().OnMouseMove(points.x, points.y);
		//	//
		//	//	//Check if we have just re-entered the window
		//	//	if (!GetMouse().IsInWindow())
		//	//	{
		//	//		SetCapture(hWnd);
		//	//		GetMouse().OnMouseEnter();
		//	//	}
		//	//}
		//	////If we arent in the region, keep capturing GetMouse() if GetMouse() key is pressed
		//	//else
		//	//{
		//	//	if (GetMouse().IsLeftPressed() || GetMouse().IsRightPressed())
		//	//		GetMouse().OnMouseMove(points.x, points.y);
		//	//	else
		//	//	{
		//	//		//release capture on not click
		//	//		ReleaseCapture();
		//	//		GetMouse().OnMouseLeave();
		//	//	}
		//	//}
		//	//break;
		//}
		//case WM_LBUTTONDOWN:
		//{
		//	//if (!bCursorEnabled)
		//	//{
		//	//	ConfineCursor(true);
		//	//	HideCursor();
		//	//}
		//	//
		//	//const POINTS points = MAKEPOINTS(lParam);
		//	//GetMouse().OnLeftPressed(points.x, points.y);
		//	//break;
		//}
		//case WM_LBUTTONUP:
		//{
		//	//const POINTS points = MAKEPOINTS(lParam);
		//	//GetMouse().OnLeftReleased(points.x, points.y);
		//	//break;
		//}
		//case WM_RBUTTONDOWN:
		//{
		//	//const POINTS points = MAKEPOINTS(lParam);
		//	//GetMouse().OnRightPressed(points.x, points.y);
		//	//break;
		//}
		//case WM_MBUTTONDOWN:
		//{
		//	//const POINTS points = MAKEPOINTS(lParam);
		//	//GetMouse().OnMiddlePressed(points.x, points.y);
		//	//break;
		//}
		//case WM_MBUTTONUP:
		//{
		//	//const POINTS points = MAKEPOINTS(lParam);
		//	//GetMouse().OnMiddleReleased(points.x, points.y);
		//	//break;
		//}
		//case WM_RBUTTONUP:
		//{
		//	//const POINTS points = MAKEPOINTS(lParam);
		//	//GetMouse().OnRightReleased(points.x, points.y);
		//	//break;
		//}
		//case WM_MOUSEWHEEL:
		//{
		//	//const POINTS points = MAKEPOINTS(lParam);
		//	//GetMouse().OnWheelDelta(points.x, points.y, GET_WHEEL_DELTA_WPARAM(wParam));
		//	//break;
		//}
		////= Raw GetMouse() Messages ==========================
		//
		//case WM_INPUT:
		//
		//	//if (!GetMouse().bRawInputEnabled)
		//	//	break;
		//	//
		//	//UINT size = 0;
		//	////Get the size of the input data
		//	//if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
		//	//	break;
		//	//
		//	//RawInputBuffer.resize(size);
		//	//
		//	////Read the input data
		//	//if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, RawInputBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
		//	//	break;
		//	//
		//	////Process the input data
		//	//auto& ri = reinterpret_cast<const RAWINPUT&>(*RawInputBuffer.data());
		//	//if (ri.header.dwType == RIM_TYPEMOUSE &&
		//	//	(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0)) //Read input from GetMouse() devices that have movement input
		//	//{
		//	//	GetMouse().OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
		//	//}
		//	//
		//	//break;
		//}
		//
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