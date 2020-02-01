#include "Flowpch.h"
#include "ImGuiLayer.h"
#include "Flow/Application.h"

#include "Flow/UserInterface/imgui/ImGui_Win32.h"
#include "Flow/UserInterface/imgui/ImGui_DX11.h"

#include "Flow/Input/KeyCodes.h"

#include "Flow/Events/MouseEvent.h"

#include "Flow/Window/WinWindow.h"
#include "Flow/Rendering/DX11/DX11RenderAPI.h"

namespace Flow
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGui Layer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsClassic();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		io.ConfigDockingWithShift = true;

		//TODO: Fix IMGUI viewports not rendering on the main window when returned to the main window
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		//ImGuiStyle& Style = ImGui::GetStyle();
		//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		//{
		//	Style.WindowRounding = 0.0f;
		//	Style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		//}

		Application& app = Application::GetApplication();
		WinWindow* Window = static_cast<WinWindow*>(&app.GetWindow());

		io.DisplaySize = ImVec2(Window->GetWidth(), Window->GetHeight());

#ifdef  FLOW_PLATFORM_WINDOWS
		ImGui_ImplWin32_Init(Window->GetWindowHandle());
		ImGui_ImplDX11_Init(RenderCommand::DX11GetDevice(), RenderCommand::DX11GetContext());
#endif

		FLOW_ENGINE_LOG("Imgui Initialised.");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_PassthruCentralNode);


		static bool bShowDemoWindow = true;
		ImGui::ShowDemoWindow(&bShowDemoWindow);
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		Application& app = Application::GetApplication();
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		//TODO: Fix IMGUI viewports not rendering on the main window when returned to the main window
		//if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		//{
		//	ImGui::UpdatePlatformWindows();
		//	ImGui::RenderPlatformWindowsDefault();
		//}
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (ImGui::GetCurrentContext() == NULL)
		{
			FLOW_ENGINE_WARNING("IMGUI context was null, unable to consume input");
			return;
		}

		EventDispatcher Dispatcher(e);
		Dispatcher.Dispatch<MouseButtonPressedEvent>(FLOW_BIND_EVENT_FUNCTION(ImGuiLayer::OnMouseButtonPressed));
		Dispatcher.Dispatch<MouseButtonReleasedEvent>(FLOW_BIND_EVENT_FUNCTION(ImGuiLayer::OnMouseButtonReleased));
		Dispatcher.Dispatch<MouseMovedEvent>(FLOW_BIND_EVENT_FUNCTION(ImGuiLayer::OnMouseMoved));
		Dispatcher.Dispatch<MouseScrolledEvent>(FLOW_BIND_EVENT_FUNCTION(ImGuiLayer::OnMouseScrolled));
		Dispatcher.Dispatch<KeyPressedEvent>(FLOW_BIND_EVENT_FUNCTION(ImGuiLayer::OnKeyPressed));
		Dispatcher.Dispatch<KeyReleasedEvent>(FLOW_BIND_EVENT_FUNCTION(ImGuiLayer::OnKeyReleased));
		Dispatcher.Dispatch<WindowResizedEvent>(FLOW_BIND_EVENT_FUNCTION(ImGuiLayer::OnWindowResized));
		Dispatcher.Dispatch<KeyTypedEvent>(FLOW_BIND_EVENT_FUNCTION(ImGuiLayer::OnKeyTyped));
	}

	bool ImGuiLayer::OnMouseButtonPressed(MouseButtonEvent& e)
	{
		//TODO: Add a window reference to the input so we know which window it was
		Application& App = Application::GetApplication();
		WinWindow* Window = static_cast<WinWindow*>(&App.GetWindow());

		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == NULL)
			::SetCapture(Window->GetWindowHandle());

		ImGuiIO& IO = ImGui::GetIO();
		IO.MouseDown[e.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.MouseDown[e.GetMouseButton()] = false;

		//TODO: Add a window reference to the input so we know which window it was
		Application& App = Application::GetApplication();
		WinWindow* Window = static_cast<WinWindow*>(&App.GetWindow());

		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == Window->GetWindowHandle())
			::ReleaseCapture();

		return false;
	}

	bool ImGuiLayer::OnMouseMoved(MouseMovedEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.MousePos = ImVec2(e.GetX(), e.GetY());
		return false;
	}

	bool ImGuiLayer::OnMouseScrolled(MouseScrolledEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.MouseWheelH += e.GetXOffset();
		IO.MouseWheel += e.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.KeysDown[e.GetKeyCode()] = true;

		IO.KeyCtrl = IO.KeysDown[VK_LCONTROL] || IO.KeysDown[VK_RCONTROL];
		IO.KeyShift = IO.KeysDown[VK_LSHIFT] || IO.KeysDown[VK_RSHIFT];
		IO.KeyCtrl = IO.KeysDown[VK_LMENU] || IO.KeysDown[VK_RMENU];
		IO.KeySuper = IO.KeysDown[VK_LWIN] || IO.KeysDown[VK_RWIN];
		return false;
	}

	bool ImGuiLayer::OnKeyTyped(KeyTypedEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.AddInputCharacter((unsigned short)e.GetKeyCode());
		return false;
	}

	bool ImGuiLayer::OnKeyReleased(KeyReleasedEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.KeysDown[e.GetKeyCode()] = false;

		IO.KeyCtrl = !IO.KeysDown[VK_LCONTROL] && !IO.KeysDown[VK_RCONTROL];
		IO.KeyShift = !IO.KeysDown[VK_LSHIFT] && !IO.KeysDown[VK_RSHIFT];
		IO.KeyCtrl = !IO.KeysDown[VK_LMENU] && !IO.KeysDown[VK_RMENU];
		IO.KeySuper = !IO.KeysDown[VK_LWIN] && !IO.KeysDown[VK_RWIN];
		return false;
	}

	bool ImGuiLayer::OnWindowResized(WindowResizedEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		IO.DisplayFramebufferScale = ImVec2(1.0f, 1.0f); //TODO: Find out what this is

		return false;
	}
}