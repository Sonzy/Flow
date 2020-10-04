#include "Flowpch.h"
#include "Editor.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow\Editor\Inspector.h"
#include "Flow\Application.h"

#include "Flow\Editor\SelectionGizmo.h"
#include "Flow/GameFramework/World.h"
#include "Flow/Editor/MenuBar.h"
#include "Flow/Editor/Windows/AssetWindow.h"

#include "ThirdParty/ImGui/imgui.h"

#include <Psapi.h> //memory debug

#include "Flow/Rendering/Other/FrameBuffer.h"
#include "Flow/Editor/EditorCamera.h"
#include "Flow/Editor/LevelManager.h"
#include "Flow/Editor/Windows/SpawnWindow.h"

#include "Flow/Editor/Toolbar.h"
#include "Flow/Editor/Tools/Tool.h"
#include "Flow/Editor/Tools/SelectionTool.h"

#define DISPATCH_TO_TOOL(ClassName)																				\
	Dispatcher.Dispatch<MouseButtonPressedEvent>(FLOW_BIND_EVENT_FUNCTION(ClassName::OnMouseButtonPressed));	\
	Dispatcher.Dispatch<MouseButtonReleasedEvent>(FLOW_BIND_EVENT_FUNCTION(ClassName::OnMouseButtonReleased));	\
	Dispatcher.Dispatch<MouseMovedEvent>(FLOW_BIND_EVENT_FUNCTION(ClassName::OnMouseMoved));					\
	Dispatcher.Dispatch<MouseScrolledEvent>(FLOW_BIND_EVENT_FUNCTION(ClassName::OnMouseScrolled));				\
	Dispatcher.Dispatch<KeyPressedEvent>(FLOW_BIND_EVENT_FUNCTION(ClassName::OnKeyPressed));					\
	Dispatcher.Dispatch<KeyTypedEvent>(FLOW_BIND_EVENT_FUNCTION(ClassName::OnKeyTyped));						\
	Dispatcher.Dispatch<KeyReleasedEvent>(FLOW_BIND_EVENT_FUNCTION(ClassName::OnKeyReleased));					\
	Dispatcher.Dispatch<WindowResizedEvent>(FLOW_BIND_EVENT_FUNCTION(ClassName::OnWindowResized));			

#define REGISTER_TOOL(NewTool) RegisterTool(new NewTool);

Editor::Editor()
	: Layer("Editor Layer")
	, m_EditorViewportSize(0,0)
	, m_Initialised(false)
{

}

Editor::~Editor()
{
	m_Tools.clear();
}

void Editor::Initialise()
{
	m_MenuBar = new MenuBar(this);
	m_Inspector = new Inspector();
	m_AssetWindow = new AssetWindow();
	m_LevelManager = new LevelManager();
	m_SpawnWindow = new SpawnWindow(World::Get());
	m_Toolbar = new ToolBar();

	REGISTER_TOOL(SelectionTool);

	m_Initialised = true;
}

void Editor::BeginPlay()
{
	for (Tool* tool : m_Tools)
	{
		tool->BeginPlay();
	}
}

void Editor::OnAttach()
{
	m_ApplicationPointer = &Application::Get();
	m_Inspector->SetCurrentWorld(World::Get());
}

void Editor::OnDetach()
{
	//TODO: Actually need to release all heap stuff, will do later since this persists through the entire program lifecycle
}

void Editor::OnImGuiRender(bool DrawEditor)
{
	//NEED TO DRAW TOOLBAR FIRST - Otherwise the toolbar gets overlapped by the dockspace.
	ImVec2 Offset = m_MenuBar->Draw();

	InitialiseDockspace(Offset);

	m_Inspector->Update();
	if (DrawEditor)
	{
		m_Inspector->Render();
		RenderApplicationDebug(m_FrameDeltaTime);
		UpdateCollisionEditor();
		m_AssetWindow->DrawWindow();
		m_SceneManager.DrawWindow_Scene();
		m_LevelManager->DrawWindows();
		m_SpawnWindow->Draw();
		//m_Console.Draw();
		

		//Draw configuration UI
		for (auto& tool : m_Tools)
		{
			if (tool->IsConfigWindowOpen())
			{
				tool->DrawConfigWindow();
			}
		}

		if (m_DrawDemoWindow)
		{
			ImGui::ShowDemoWindow(&m_DrawDemoWindow);
		}			
	}
}

void Editor::OnEvent(Event& e)
{
	EventDispatcher Dispatcher(e);
	Dispatcher.Dispatch<MouseButtonPressedEvent>(FLOW_BIND_EVENT_FUNCTION(Editor::OnMouseButtonPressed));
	Dispatcher.Dispatch<MouseButtonReleasedEvent>(FLOW_BIND_EVENT_FUNCTION(Editor::OnMouseButtonReleased));
	Dispatcher.Dispatch<MouseMovedEvent>(FLOW_BIND_EVENT_FUNCTION(Editor::OnMouseMoved));
	Dispatcher.Dispatch<MouseScrolledEvent>(FLOW_BIND_EVENT_FUNCTION(Editor::OnMouseScrolled));
	Dispatcher.Dispatch<KeyPressedEvent>(FLOW_BIND_EVENT_FUNCTION(Editor::OnKeyPressed));
	Dispatcher.Dispatch<KeyTypedEvent>(FLOW_BIND_EVENT_FUNCTION(Editor::OnKeyTyped));
	Dispatcher.Dispatch<KeyReleasedEvent>(FLOW_BIND_EVENT_FUNCTION(Editor::OnKeyReleased));
	Dispatcher.Dispatch<WindowResizedEvent>(FLOW_BIND_EVENT_FUNCTION(Editor::OnWindowResized));

	for (Tool* tool : m_Tools)
	{
				if (EventDispatcher::IsMatchingFunction<MouseButtonPressedEvent>(e))	tool->OnMouseButtonPressed(static_cast<MouseButtonPressedEvent&>(e));
		else	if (EventDispatcher::IsMatchingFunction<MouseButtonReleasedEvent>(e))	tool->OnMouseButtonReleased(static_cast<MouseButtonReleasedEvent&>(e));
		else	if (EventDispatcher::IsMatchingFunction<MouseMovedEvent>(e))			tool->OnMouseMoved(static_cast<MouseMovedEvent&>(e));
		else	if (EventDispatcher::IsMatchingFunction<MouseScrolledEvent>(e))			tool->OnMouseScrolled(static_cast<MouseScrolledEvent&>(e));
		else	if (EventDispatcher::IsMatchingFunction<KeyPressedEvent>(e))			tool->OnKeyPressed(static_cast<KeyPressedEvent&>(e));
		else	if (EventDispatcher::IsMatchingFunction<KeyTypedEvent>(e))				tool->OnKeyTyped(static_cast<KeyTypedEvent&>(e));
		else	if (EventDispatcher::IsMatchingFunction<KeyReleasedEvent>(e))			tool->OnKeyReleased(static_cast<KeyReleasedEvent&>(e));
		else	if (EventDispatcher::IsMatchingFunction<WindowResizedEvent>(e))			tool->OnWindowResized(static_cast<WindowResizedEvent&>(e));
	}
}

void Editor::OnUpdate(float DeltaTime)
{
	m_FrameDeltaTime = DeltaTime;

	if (!m_EditorCam)
		m_EditorCam = std::dynamic_pointer_cast<EditorCamera>(RenderCommand::GetMainCamera());

	//_SelectionGizmo->Render();

	UpdateTools(DeltaTime);
	RenderTools();
}

void Editor::UpdateTools(float DeltaTime)
{
	for (Tool* tool : m_Tools)
	{
		tool->UpdateTool(DeltaTime);
	}
}

void Editor::RenderTools()
{
	for (Tool* tool : m_Tools)
	{
		tool->RenderTool();
	}
}

void Editor::RegisterTool(Tool* newTool)
{
	if (std::find(m_Tools.begin(), m_Tools.end(), newTool) != m_Tools.end())
	{
		FLOW_ENGINE_WARNING("Editor::RegisterTool: Already registered tool");
	}

	m_Tools.push_back(newTool);
}

Editor* Editor::GetEditor()
{
	return Application::Get().GetEditor();
}

Editor::Settings& Editor::GetEditorSettings()
{
	return Editor::GetEditor()->m_Settings;
}

Inspector* Editor::GetInspector() const
{
	return m_Inspector;
}

MenuBar* Editor::GetMenuBar() const
{
	return m_MenuBar;
}

void Editor::SetDemoWindowVisible(bool Enabled)
{
	m_DrawDemoWindow = Enabled;
}

void Editor::ToggleImGuiDemoWindow()
{
	m_DrawDemoWindow = !m_DrawDemoWindow;
}

void Editor::Open_NewLevelWindow()
{
	m_LevelManager->Open_NewLevelWindow();
}

Console& Editor::GetConsole()
{
	return m_Console;
}

bool Editor::IsSceneWindowFocused() const
{
	return m_SceneManager.IsSceneWindowFocused();
}

bool Editor::IsMouseOverScene() const
{
	return m_SceneManager.IsMouseOverScene();
}

IntVector2 Editor::GetSceneWindowSize() const
{
	return m_SceneManager.GetSceneWindowSize();
}

IntVector2 Editor::GetSceneWindowPosition() const
{
	return m_SceneManager.GetSceneWindowPosition();
}

bool Editor::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	bool Handled = false;

	CONSUMES_INPUT(m_EditorCam, m_EditorCam->OnMouseButtonPressed(e));

	if (m_Inspector && m_Inspector->OnMouseClicked(e))
		return true;

	return false;
}

bool Editor::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
{
	if (m_EditorCam && m_EditorCam->OnMouseButtonReleased(e))
		return true;

	if (m_Inspector && m_Inspector->OnMouseReleased(e))
		return true;

	return false;
}

bool Editor::OnMouseMoved(MouseMovedEvent& e)
{
	return false;
}

bool Editor::OnMouseScrolled(MouseScrolledEvent& e)
{
	if (m_EditorCam && m_EditorCam->OnMouseScrolled(e))
		return true;

	return false;
}

bool Editor::OnKeyPressed(KeyPressedEvent& e)
{
	if (m_EditorCam && m_EditorCam->OnKeyPressed(e))
		return true;

	if (m_Inspector && m_Inspector->OnKeyPressed(e))
		return true;

	return false;
}

bool Editor::OnKeyTyped(KeyTypedEvent& e)
{
	return false;
}

bool Editor::OnKeyReleased(KeyReleasedEvent& e)
{
	if (m_EditorCam && m_EditorCam->OnKeyReleased(e))
		return true;

	return false;
}

bool Editor::OnWindowResized(WindowResizedEvent& e)
{
	return false;
}

void Editor::UpdateCollisionEditor()
{
	//if (!_CollisionWindowOpen)
	//	return;
	
	//if(ImGui::Begin("Collision Editor", &_CollisionWindowOpen))
	//{
	//
	//}
	//ImGui::End();
}



void Editor::InitialiseDockspace(ImVec2 Offset)
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

void Editor::RenderApplicationDebug(float DeltaTime)
{
	m_TimeSinceFrameUpdate += DeltaTime;
	m_FrameCounter++;

	if (m_TimeSinceFrameUpdate >= m_UpdateInterval)
	{
		m_FrameTimer = m_TimeSinceFrameUpdate / m_FrameCounter;
		m_TimeSinceFrameUpdate = 0;
		m_FrameCounter = 0;
		m_LastFrameTime = DeltaTime * 1000;
	}

	if (ImGui::Begin("Application Statistics"))
	{
		ImGui::Checkbox("Pause Game", &m_ApplicationPointer->m_GamePaused);
		ImGui::Checkbox("Draw Collision", &m_ApplicationPointer->m_DrawCollision);

		ImGui::Text("Framerate: %.1f", 1 / m_FrameTimer);
		ImGui::Text("FrameTime: %.1f ms", m_LastFrameTime);

		PROCESS_MEMORY_COUNTERS MemoryData;
		GetProcessMemoryInfo(GetCurrentProcess(), &MemoryData, sizeof(MemoryData));

		ImGui::Text("Memory: %.1f MB", (float)MemoryData.WorkingSetSize / 1048576);
	}
	ImGui::End();

	if (ImGui::Begin("Rendering Configuration"))
	{
		RenderQueue* Queue = RenderQueue::Get();
		ImGui::Checkbox("Pass 0", &Queue->m_Pass0Enabled);
		ImGui::Checkbox("Pass 1", &Queue->m_Pass1Enabled);
		ImGui::Checkbox("Pass 2", &Queue->m_Pass2Enabled);
		ImGui::Checkbox("Pass 3", &Queue->m_Pass3Enabled);
		ImGui::Checkbox("Pass 4", &Queue->m_Pass4Enabled);
		ImGui::Checkbox("Pass 5", &Queue->m_Pass5Enabled);
	}
	ImGui::End();
}