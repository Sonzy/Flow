#include "Flowpch.h"
#include "EditorLayer.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow\Editor\Inspector.h"
#include "Flow\Application.h"

#include "Flow\Editor\SelectionGizmo.h"
#include "Flow/GameFramework/World.h"
#include "Flow/Editor/Toolbar.h"
#include "Flow/Editor/Windows/AssetWindow.h"

#include "ThirdParty/ImGui/imgui.h"

#include <Psapi.h> //memory debug

#include "Flow/Editor/Windows/CollisionEditor.h"

#include "Flow/Rendering/Other/FrameBuffer.h"
#include "Flow/Editor/EditorCamera.h"


EditorLayer::EditorLayer()
	: Layer("Editor Layer"), _EditorViewportSize(0,0)
{
}

void EditorLayer::BeginPlay()
{
	_SelectionGizmo->InitialisePhysics();
}

void EditorLayer::OnAttach()
{
	_SelectionGizmo = new SelectionGizmo();
	_SelectionGizmo->GenerateCollision();

	_Toolbar = new Toolbar(this);

	_Inspector = new Inspector(_SelectionGizmo);
	_Inspector->SetCurrentWorld(World::GetWorld());

	_ApplicationPointer = &Application::GetApplication();

	_AssetWindow = new AssetWindow();
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnImGuiRender(bool DrawEditor)
{
	//NEED TO DRAW TOOLBAR FIRST - Otherwise the toolbar gets overlapped by the dockspace.
	ImVec2 Offset = _Toolbar->Draw();

	InitialiseDockspace(Offset);

	_Inspector->Update();
	if (DrawEditor)
	{
		_Inspector->Render();
		RenderApplicationDebug(FrameDeltaTime);
		UpdateCollisionEditor();
		_AssetWindow->DrawWindow();
		DrawSceneWindow();

		if (_DrawDemoWindow)
			ImGui::ShowDemoWindow(&_DrawDemoWindow);
	}
}

void EditorLayer::OnEvent(Event& e)
{
	EventDispatcher Dispatcher(e);
	Dispatcher.Dispatch<MouseButtonPressedEvent>(FLOW_BIND_EVENT_FUNCTION(EditorLayer::OnMouseButtonPressed));
	Dispatcher.Dispatch<MouseButtonReleasedEvent>(FLOW_BIND_EVENT_FUNCTION(EditorLayer::OnMouseButtonReleased));
	Dispatcher.Dispatch<MouseMovedEvent>(FLOW_BIND_EVENT_FUNCTION(EditorLayer::OnMouseMoved));
	Dispatcher.Dispatch<MouseScrolledEvent>(FLOW_BIND_EVENT_FUNCTION(EditorLayer::OnMouseScrolled));
	Dispatcher.Dispatch<KeyPressedEvent>(FLOW_BIND_EVENT_FUNCTION(EditorLayer::OnKeyPressed));
	Dispatcher.Dispatch<KeyTypedEvent>(FLOW_BIND_EVENT_FUNCTION(EditorLayer::OnKeyTyped));
	Dispatcher.Dispatch<KeyReleasedEvent>(FLOW_BIND_EVENT_FUNCTION(EditorLayer::OnKeyReleased));
	Dispatcher.Dispatch<WindowResizedEvent>(FLOW_BIND_EVENT_FUNCTION(EditorLayer::OnWindowResized));

}

void EditorLayer::OnUpdate(float DeltaTime)
{
	FrameDeltaTime = DeltaTime;

	if (!_EditorCam)
		_EditorCam = std::dynamic_pointer_cast<EditorCamera>(RenderCommand::GetMainCamera());

	_SelectionGizmo->Render();
}

EditorLayer* EditorLayer::GetEditor()
{
	return Application::GetApplication().GetEditor();
}

EditorSettings& EditorLayer::GetEditorSettings()
{
	return EditorLayer::GetEditor()->_Settings;
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

void EditorLayer::OpenCollisionEditor()
{
	_EditorWindows.push_back(std::make_shared<CollisionEditor>());
}

bool EditorLayer::IsSceneWindowFocused() const
{
	return _SceneWindowFocused;
}

bool EditorLayer::IsMouseOverScene() const
{
	return _MouseOverScene;
}

IntVector2D EditorLayer::GetSceneWindowSize() const
{
	return _SceneWindowSize;
}

IntVector2D EditorLayer::GetSceneWindowPosition() const
{
	return _SceneWindowPosition;
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	bool Handled = false;

	CONSUMES_INPUT(_EditorCam, _EditorCam->OnMouseButtonPressed(e));

	if (_Inspector && _Inspector->OnMouseClicked(e))
		return true;

	return false;
}

bool EditorLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
{
	if (_EditorCam && _EditorCam->OnMouseButtonReleased(e))
		return true;

	if (_Inspector && _Inspector->OnMouseReleased(e))
		return true;

	return false;
}

bool EditorLayer::OnMouseMoved(MouseMovedEvent& e)
{
	return false;
}

bool EditorLayer::OnMouseScrolled(MouseScrolledEvent& e)
{
	if (_EditorCam && _EditorCam->OnMouseScrolled(e))
		return true;

	return false;
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
{
	if (_EditorCam && _EditorCam->OnKeyPressed(e))
		return true;

	return false;
}

bool EditorLayer::OnKeyTyped(KeyTypedEvent& e)
{
	return false;
}

bool EditorLayer::OnKeyReleased(KeyReleasedEvent& e)
{
	if (_EditorCam && _EditorCam->OnKeyReleased(e))
		return true;

	return false;
}

bool EditorLayer::OnWindowResized(WindowResizedEvent& e)
{
	return false;
}

void EditorLayer::UpdateCollisionEditor()
{
	//if (!_CollisionWindowOpen)
	//	return;
	
	//if(ImGui::Begin("Collision Editor", &_CollisionWindowOpen))
	//{
	//
	//}
	//ImGui::End();
}

void EditorLayer::DrawSceneWindow()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	auto State = World::GetWorld()->GetWorldState();
	std::string WindowName;
	switch (State)
	{
	case WorldState::Paused: WindowName = "Scene - Paused###Scene"; break;
	case WorldState::Editor: WindowName = "Scene###Scene"; break;
	case WorldState::InGame: WindowName = "Scene - Playing###Scene"; break;
	}

	if (ImGui::Begin(WindowName.c_str()))
	{
		_SceneWindowFocused = ImGui::IsWindowFocused();
		if (_EditorCam)
			_EditorCam->_CanUpdate = _SceneWindowFocused ? true : !ImGui::IsAnyItemActive();

		FrameBuffer* Buff = RenderCommand::GetEditorFrameBuffer();
			
		//Note: We assume that the scene image has no padding and is flush to the window x. 
		_SceneWindowPosition = IntVector2D(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y);
		_SceneWindowSize = IntVector2D(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		_MouseOverScene = ImGui::IsWindowHovered();

		if (*reinterpret_cast<Vector2D*>(&_SceneWindowSize) != _EditorViewportSize)//TODO: Stop being naughty
		{
			Buff->Resize(_SceneWindowSize.X, _SceneWindowSize.Y);
			_EditorViewportSize = *reinterpret_cast<Vector2D*>(&_SceneWindowSize);
		}
	
		ImGui::Image(Buff->GetTextureView(), ImVec2(Buff->GetWidth(), Buff->GetHeight()));
	}
	ImGui::End();

	ImGui::PopStyleVar();
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

#include "Flow/Rendering/Core/RenderQueue/RenderQueue.h"

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

	if (ImGui::Begin("Rendering Configuration"))
	{
		RenderQueue* Queue = RenderQueue::Get();
		ImGui::Checkbox("Pass 0", &Queue->_Pass0Enabled);
		ImGui::Checkbox("Pass 1", &Queue->_Pass1Enabled);
		ImGui::Checkbox("Pass 2", &Queue->_Pass2Enabled);
		ImGui::Checkbox("Pass 3", &Queue->_Pass3Enabled);
		ImGui::Checkbox("Pass 4", &Queue->_Pass4Enabled);
	}
	ImGui::End();
}