#include "Flowpch.h"
#include "ImGuiLayer.h"
#include "Flow/Application.h"

#include "Flow/UserInterface/imgui/ImGui_Win32.h"
#include "Flow/UserInterface/imgui/ImGui_DX11.h"
#include "imgui.h"

#include "Flow/Events/MouseEvent.h"

#include "Flow/Window/WinWindow.h"
#include "Flow/Rendering/DX11/GraphicsDX11.h"

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
		ImGui::StyleColorsLight();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		//TODO: check if I need to set up the keybinds

		Application& app = Application::GetApplication();

		WinWindow* Window = dynamic_cast<WinWindow*>(&app.GetWindow());
		
		if (!Window)
		{
			FLOW_ENGINE_ERROR("ImGuiLayer::OnAttach: Failed to get window as Windows window");
			return;
		}

		ImGui_ImplWin32_Init(Window->GetWindowHandle());
		ImGui_ImplDX11_Init(Window->Gfx().GetDevice(), Window->Gfx().GetContext());

		FLOW_ENGINE_LOG("Imgui Initialised.");
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		Application& app = Application::GetApplication();
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		static bool bShowDemoWindow = true;
		ImGui::ShowDemoWindow(&bShowDemoWindow);

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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
		ImGuiIO& IO = ImGui::GetIO();
		IO.MouseDown[e.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& IO = ImGui::GetIO();
		IO.MouseDown[e.GetMouseButton()] = false;

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
		IO.AddInputCharacter((unsigned short) e.GetKeyCode());
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