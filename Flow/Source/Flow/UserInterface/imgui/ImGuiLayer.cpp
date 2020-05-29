#include "Flowpch.h"
#include "ImGuiLayer.h"
#include "Flow/Application.h"

#include "Flow/UserInterface/imgui/ImGui_Win32.h"
#include "Flow/UserInterface/imgui/ImGui_DX11.h"

#include "Flow/Input/KeyCodes.h"

#include "Flow/Events/MouseEvent.h"

#include "Flow/Window/WinWindow.h"
#include "Flow/Rendering/DX11/DX11RenderAPI.h"

ImGuiLayer::ImGuiLayer()
	: Layer("ImGui Layer")
{
}

ImGuiLayer::~ImGuiLayer()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
}

void ImGuiLayer::OnAttach()
{
	ImGui::CreateContext();
	ConfigureStyle();

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

#ifdef  FLOW_PLATFORM_WINDOWS
	ImGui_ImplWin32_Init(Window->GetWindowHandle());
	ImGui_ImplDX11_Init(RenderCommand::DX11GetDevice(), RenderCommand::DX11GetContext());
#endif

	RECT rect;
	::GetClientRect(Window->GetWindowHandle(), &rect);
	io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

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
	//static bool bShowDemoWindow = true;
	//ImGui::ShowDemoWindow(&bShowDemoWindow);
}

void ImGuiLayer::Begin()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_PassthruCentralNode);
}

void ImGuiLayer::End()
{
	//Application& app = Application::GetApplication();
	//ImGuiIO& io = ImGui::GetIO();
	//io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

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

	if (IO.WantCaptureMouse)
		return true;

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
	IO.MousePos = ImVec2((float)e.GetX(), (float)e.GetY());
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

	return false;
}

void ImGuiLayer::ConfigureStyle()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.47f, 0.69f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.42f, 0.41f, 0.64f, 0.69f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.99f, 0.40f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.99f, 0.40f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.51f, 0.21f, 0.00f, 0.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.80f, 0.32f, 0.00f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.32f, 0.00f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.41f, 0.39f, 0.80f, 0.60f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.41f, 0.39f, 0.80f, 0.60f);
	colors[ImGuiCol_Button] = ImVec4(0.99f, 0.40f, 0.00f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.48f, 0.71f, 0.79f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.46f, 0.54f, 0.80f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.99f, 0.40f, 0.00f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.99f, 0.59f, 0.00f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
	colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.90f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.16f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.78f, 0.82f, 1.00f, 0.60f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.82f, 1.00f, 0.90f);
	colors[ImGuiCol_Tab] = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.40f, 0.40f, 0.90f, 0.31f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}