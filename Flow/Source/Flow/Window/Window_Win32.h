#pragma once

//= Includes ====================================================

#include "Flow/Window/Window.h"

//= Class Definition ============================================

/* Window class for windows platform */
class Window_Win32 : public Window
{
public:

	//= Public Functions ================================

								Window_Win32(const Window::Properties& Properties, bool MainWindow);
	virtual						~Window_Win32();

	void						PreUpdate() override;
	void						OnUpdate() override;
	void						PostUpdate() override;
	virtual void				Shutdown() override;

	unsigned int				GetWidth() const override;
	unsigned int				GetHeight() const override;

	void						SetWindowSizeWithAdjust(RECT& rect) const;

	void						SetEventCallback(const EventCallbackFunction& Callback) override;

	void						EnableVSync(bool bEnabled) override;
	bool						IsVSyncEnabled() const override;

	static std::optional<int>	ProcessWindowsMessages();

	HWND&						GetWindowHandle();

	void						Resize(int Width, int Height) override;

	static IntVector2			GetAdjustedWindowSize();

private:

	//= Private Structs ==================================

	struct WindowData
	{
		std::string				Title;
		unsigned int			Width;
		unsigned int			Height;
		EventCallbackFunction	EventCallback;
	};

private:
	friend class WindowClass;
	
	//= Private Functions ================================

	virtual void				Initialise(const Window::Properties& Properties);

	static LRESULT WINAPI		HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI		HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT						HandleMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

	//= Private Variables ================================

	HWND						m_WindowHandle;
	DWORD						m_WindowStyle;
	WindowData					m_WindowData;
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