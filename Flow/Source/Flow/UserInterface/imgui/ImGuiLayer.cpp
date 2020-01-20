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
		//ImGuiIO& io = ImGui::GetIO();
		//io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

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

		Application& app = Application::GetApplication();
		WinWindow* Window = dynamic_cast<WinWindow*>(&app.GetWindow());

		ImGuiIO& IO = ImGui::GetIO();

		switch (e.GetEventType())
		{
		case EventType::MouseButtonPressed:
		{
			MouseButtonPressedEvent* MousePressedEvent = dynamic_cast<MouseButtonPressedEvent*>(&e);
			if (!ImGui::IsAnyMouseDown() && ::GetCapture() == NULL)
				::SetCapture(Window->GetWindowHandle());

			IO.MouseDown[MousePressedEvent->GetMouseButton()] = true;
			e.bHandled = true;
			break;
		}
		case EventType::MouseButtonReleased:

			MouseButtonReleasedEvent* MouseReleasedEvent = dynamic_cast<MouseButtonReleasedEvent*>(&e);

			IO.MouseDown[MouseReleasedEvent->GetMouseButton()] = false;

			if (!ImGui::IsAnyMouseDown() && ::GetCapture() == Window->GetWindowHandle())
				::ReleaseCapture();
			e.bHandled = true;
			break;
		}

	}
}