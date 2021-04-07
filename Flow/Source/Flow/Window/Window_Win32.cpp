#include "pch.h"
#include "Window_Win32.h"
#include "Flow/Core.h"
#include "Framework/Events/ApplicationEvent.h"
#include "Framework/Events/MouseEvent.h"
#include "Framework/Events/KeyEvent.h"
#include "Rendering/Renderer.h"
#include "Application.h"

Window* Window::Create(const Window::Properties& Properties, bool MainWindow)
{
	return new Window_Win32(Properties, MainWindow);
}

Window_Win32::Window_Win32(const Window::Properties& Properties, bool MainWindow)
{
	Initialise(Properties);

	m_MainWindow = MainWindow;

	//TODO: Clean this up, we pass in the adjusted window rect value
	RECT rect;
	::GetClientRect(m_WindowHandle, &rect);
	IntVector2 Adj = IntVector2(float(rect.right - rect.left), float(rect.bottom - rect.top));

	if(m_MainWindow)
		RenderCommand::InitialiseDX11(GetWindowHandle(), Adj.x, Adj.y);
}

Window_Win32::~Window_Win32()
{
	Shutdown();
}

void Window_Win32::Initialise(const Window::Properties& Properties)
{
	bool result;

	RECT WindowRegion;
	WindowRegion.left = 100;
	WindowRegion.right = Properties.m_Width + WindowRegion.left;
	WindowRegion.top = 100;
	WindowRegion.bottom = Properties.m_Height + WindowRegion.top;

	m_WindowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;

	result = AdjustWindowRect(&WindowRegion, m_WindowStyle, TRUE);
	FLOW_ASSERT(result, "Window_Win32::Initialise: Failed to adjust the window rect");

	m_WindowHandle = CreateWindowEx(
		0,
		WindowClass::GetName(),
		Properties.m_Title.c_str(),
		m_WindowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WindowRegion.right - WindowRegion.left,
		WindowRegion.bottom - WindowRegion.top,
		nullptr,
		nullptr,
		WindowClass::GetInstanceHandle(),
		this
	);

	ShowWindow(m_WindowHandle, SW_SHOW);

	m_Props = Properties;

	m_WindowData.Title = m_Props.m_Title;
	m_WindowData.Width = m_Props.m_Width;
	m_WindowData.Height = m_Props.m_Height;
}

void Window_Win32::Shutdown()
{
	DestroyWindow(m_WindowHandle);
}

void Window_Win32::PreUpdate()
{
	RenderCommand::BeginFrame();
}

void Window_Win32::OnUpdate()
{
	ProcessWindowsMessages();
}

void Window_Win32::PostUpdate()
{
	RenderCommand::EndFrame();
}

unsigned int Window_Win32::GetWidth() const
{
	return m_Props.m_Width;
}

unsigned int Window_Win32::GetHeight() const
{
	return m_Props.m_Height;
}

void Window_Win32::SetWindowSizeWithAdjust(RECT& rect) const
{
	::AdjustWindowRectEx(&rect, 0, FALSE, m_WindowStyle);
}

void Window_Win32::SetEventCallback(const EventCallbackFunction& Callback)
{
	m_WindowData.EventCallback = Callback;
}

void Window_Win32::EnableVSync(bool bEnabled)
{
	FLOW_ENGINE_LOG("TODO: Window_Win32::EnableVSync");
}

bool Window_Win32::IsVSyncEnabled() const
{
	FLOW_ENGINE_LOG("TODO: Window_Win32::IsVSyncEnabled");
	return false;
}

LRESULT __stdcall Window_Win32::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg != WM_NCCREATE)
		return DefWindowProc(hWnd, msg, wParam, lParam);

	//Get window pointer from creation data
	const CREATESTRUCTW* const creationStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
	Window_Win32* const WindowPtr = static_cast<Window_Win32*>(creationStruct->lpCreateParams);

	//Store pointer to window class in the WinApi user data
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(WindowPtr));
	//Set the windows message handling to use my function
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window_Win32::HandleMsgThunk));

	return WindowPtr->HandleMessages(hWnd, msg, wParam, lParam);
}

LRESULT __stdcall Window_Win32::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Pass the message to the window message handler
	Window_Win32* const window = reinterpret_cast<Window_Win32*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return window->HandleMessages(hWnd, msg, wParam, lParam);
}

LRESULT Window_Win32::HandleMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
	case WM_SIZE:
	{
		int ResizeType = static_cast<int>(wParam);
		switch (ResizeType)
		{
		case SIZE_MAXIMIZED:
		case SIZE_RESTORED:
		{
			WindowResizedEvent Event(LOWORD(lParam), HIWORD(lParam));
			e = &Event;
			break;
		}
		case SIZE_MINIMIZED:
		{
			WindowMinimizedEvent Event = WindowMinimizedEvent();
			e = &Event;
			break;
		}
		default:
			FLOW_ENGINE_WARNING("Unhandled Resize type %d", ResizeType);
			break;
		}

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
		KeyPressedEvent Event(static_cast<int>(wParam), 0);
		e = &Event;
		break;
	}
	case WM_KEYUP:
	{
		KeyReleasedEvent Event(static_cast<int>(wParam));
		e = &Event;
		break;
	}
	case WM_CHAR:
	{
		KeyTypedEvent Event(static_cast<unsigned char>(wParam));
		e = &Event;
		break;
	}
	//= Other =
	default:
		bHandled = false;
	}

	if (e == nullptr)
	{
		if (bHandled == true)
		{
			FLOW_ENGINE_ERROR("Window_Win32::HandleMessages: Error was nullptr (Message: %d)", msg);
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	if (m_WindowData.EventCallback == nullptr)
	{
		if (const bool ThrowError = false)
		{
			FLOW_ENGINE_ERROR("Window_Win32::HandleMessages: No callback assigned (Message: %d)", msg);
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	//If we have a handled event, call the callback
	m_WindowData.EventCallback(*e);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


std::optional<int> Window_Win32::ProcessWindowsMessages()
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

HWND& Window_Win32::GetWindowHandle()
{
	return m_WindowHandle;
}

void Window_Win32::Resize(int Width, int Height)
{

}

IntVector2 Window_Win32::GetAdjustedWindowSize()
{
	Window_Win32* Win = static_cast<Window_Win32*>(&Application::Get().GetWindow());
	RECT rect;
	::GetClientRect(Win->GetWindowHandle(), &rect);
	return IntVector2(float(rect.right - rect.left), float(rect.bottom - rect.top));
}

bool Window_Win32::OpenFile(std::string& outPath)
{
	//TODO: Actually configure the settings, works with these defaults for now
	OPENFILENAME ofn = { 0 };
	TCHAR szFile[260] = { 0 };

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetWindowHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = ".FMESH\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		outPath = ofn.lpstrFile;
		return true;
	}

	return false;
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
	: InstanceHandle(GetModuleHandle(nullptr))
{
	WNDCLASSEX windowsClass = { 0 };
	windowsClass.cbSize = sizeof(windowsClass);
	windowsClass.style = CS_OWNDC;
	windowsClass.lpfnWndProc = Window_Win32::HandleMessageSetup; //Link to the function that will be handling windows messages
	windowsClass.cbClsExtra = 0;
	windowsClass.cbWndExtra = 0;
	windowsClass.hInstance = GetInstanceHandle();
	windowsClass.hIcon = static_cast<HICON>(LoadImage(windowsClass.hInstance, MAKEINTRESOURCE(101), IMAGE_ICON, 128, 128, 0)); //Used to set up custom icon
	windowsClass.hIconSm = static_cast<HICON>(LoadImage(windowsClass.hInstance, MAKEINTRESOURCE(101), IMAGE_ICON, 32, 32, 0));// static_cast<HICON>(LoadImage(windowsClass.hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));; //Same as before but small one
	windowsClass.hCursor = LoadCursor(NULL, IDC_ARROW); ;
	windowsClass.lpszClassName = ClassName;	

	DWORD error = GetLastError();
	
	RegisterClassEx(&windowsClass);
}
WindowClass::~WindowClass()
{
	UnregisterClass(ClassName, GetInstanceHandle());
}