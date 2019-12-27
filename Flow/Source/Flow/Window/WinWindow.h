#pragma once
#include "Flow/Window/Window.h"

namespace Flow
{
	/* Window class for windows platform */
	class WinWindow : public Window
	{
	public:
		WinWindow(const WindowProperties& Properties);
		virtual ~WinWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override;
		unsigned int GetHeight() const override;

		void SetEventCallback(const EventCallbackFunction& Callback) override;

		void EnableVSync(bool bEnabled) override;
		bool IsVSyncEnabled() const override;

		static std::optional<int> ProcessWindowsMessages();

		HWND& GetWindowHandle();
	private:
		friend class WindowClass;

		virtual void Initialise(const WindowProperties& Properties);
		virtual void Shutdown();

		static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:

		HWND WindowHandle;

		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			EventCallbackFunction EventCallback;
		};

		WindowData m_WindowData;
	};

	class WindowClass
	{
	public:
		static const char* GetName();
		static HINSTANCE GetInstanceHandle();

	private:
		WindowClass();
		~WindowClass();

		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

	private:
		static constexpr const char* ClassName = "Flow Window Class";
		static WindowClass WindowClassInstance;
		HINSTANCE InstanceHandle;
	};
}