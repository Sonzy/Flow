#include "Flowpch.h"
#include "ImGuiLayer.h"
#include "Flow/Application.h"

#include "Flow/UserInterface/imgui/ImGui_Win32.h"
#include "Flow/UserInterface/imgui/ImGui_DX11.h"
#include "imgui.h"

#include "Flow/Window/WinWindow.h"

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
		io.Fonts->Build();
		//ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
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
	}
}