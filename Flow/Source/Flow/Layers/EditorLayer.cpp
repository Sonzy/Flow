#include "Flowpch.h"
#include "EditorLayer.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow\Editor\Inspector.h"
#include "Flow\Application.h"

#include "Flow\Editor\SelectionGizmo.h"
#include "Flow/GameFramework/World.h"
#include "Flow/Editor/Toolbar.h"

#include "ThirdParty/ImGui/imgui.h"

#include <Psapi.h> //memory debug

EditorLayer::EditorLayer()
	: Layer("Editor Layer")
{
}

void EditorLayer::OnAttach()
{
	//_SelectionGizmo = new SelectionGizmo();
	//_SelectionGizmo->GenerateCollision();
	//_SelectionGizmo->AddCollidersToWorld(GameWorld_);

	_Toolbar = new Toolbar(this);

	_Inspector = new Inspector(_SelectionGizmo);
	_Inspector->SetCurrentWorld(World::GetWorld());

	_ApplicationPointer = &Application::GetApplication();
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnImGuiRender(bool DrawEditor)
{
	//NEED TO DRAW TOOLBAR FIRST - Otherwise the toolbar gets overlapped by the dockspace.
	ImVec2 Offset = _Toolbar->Draw();

	InitialiseDockspace(Offset);

	if (DrawEditor)
	{
		_Inspector->Update();

		RenderApplicationDebug(FrameDeltaTime);

		if (_DrawDemoWindow)
			ImGui::ShowDemoWindow(&_DrawDemoWindow);
	}
}

void EditorLayer::OnEvent(Event& e)
{
	EventDispatcher Dispatcher(e);
	Dispatcher.Dispatch<MouseButtonPressedEvent>(FLOW_BIND_EVENT_FUNCTION(EditorLayer::OnMouseButtonPressed));
}

void EditorLayer::OnUpdate(float DeltaTime)
{
	FrameDeltaTime = DeltaTime;
}

EditorLayer* EditorLayer::GetEditor()
{
	return Application::GetApplication().GetEditor();
}

Inspector* EditorLayer::GetInspector() const
{
	return _Inspector;
}

Toolbar* EditorLayer::GetToolbar() const
{
	return _Toolbar;
}

void EditorLayer::SetDemoWindowVisible(bool Enabled)
{
	_DrawDemoWindow = Enabled;
}

void EditorLayer::ToggleImGuiDemoWindow()
{
	_DrawDemoWindow = !_DrawDemoWindow;
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	if (_Inspector)
		return _Inspector->OnMouseClicked(e);

	return false;
}

void EditorLayer::InitialiseDockspace(ImVec2 Offset)
{
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + Offset.y));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - Offset.y));
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags host_window_flags = 0;
	host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
	host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	host_window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Example DockSpace", NULL, host_window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, nullptr);
	ImGui::End();
}

void EditorLayer::RenderApplicationDebug(float DeltaTime)
{
	TimeSinceFrameUpdate += DeltaTime;
	FrameCounter++;

	if (TimeSinceFrameUpdate >= UpdateInterval)
	{
		FrameTimer = TimeSinceFrameUpdate / FrameCounter;
		TimeSinceFrameUpdate = 0;
		FrameCounter = 0;
		LastFrameTime = DeltaTime * 1000;
	}

	if (ImGui::Begin("Application Statistics"))
	{
		ImGui::Checkbox("Pause Game", &_ApplicationPointer->Paused_);
		ImGui::Checkbox("Draw Collision", &_ApplicationPointer->DrawCollision_);

		ImGui::Text("Framerate: %.1f", 1 / FrameTimer);
		ImGui::Text("FrameTime: %.1f ms", LastFrameTime);

		PROCESS_MEMORY_COUNTERS MemoryData;
		GetProcessMemoryInfo(GetCurrentProcess(), &MemoryData, sizeof(MemoryData));

		ImGui::Text("Memory: %.1f MB", (float)MemoryData.WorkingSetSize / 1048576);
	}
	ImGui::End();
}