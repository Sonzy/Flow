#include "Flowpch.h"
#include "Editor.h"
#include "Events/MouseEvent.h"
#include "Editor/UIComponents/Inspector.h"
#include "Application.h"

#include "Editor\SelectionGizmo.h"
#include "GameFramework/World.h"
#include "Editor/MenuBar.h"
#include "Editor/UIComponents/AssetBrowser.h"

#include "ThirdParty/ImGui/imgui.h"

#include <Psapi.h> //memory debug
#include "Flow/Rendering/Core/RenderQueue/RenderQueue.h"

#include "Rendering/Other/FrameBuffer.h"
#include "Editor/EditorCamera.h"
#include "Editor/UIComponents/LevelManager.h"
#include "Editor/UIComponents/SpawnWindow.h"
#include "Editor/UIComponents/Console.h"

#include "Editor/UIComponents/Toolbar.h"
#include "Editor/Tools/Tool.h"
#include "Editor/Tools/SelectionTool.h"

#define PASS_KEY_EVENT(MemberName) 	if (MemberName != nullptr && MemberName->OnKeyPressed(e)) { return true; }

Editor::Editor()
	: Layer("Editor Layer")
	, m_EditorViewportSize(0,0)
	, m_Initialised(false)
	, m_ShowSettingsWindow(false)
{

}

Editor::~Editor()
{
	m_Tools.clear();
}

void Editor::Initialise()
{
	m_MenuBar = new MenuBar(this);

	RegisterTool<SelectionTool>();

	RegisterUIComponent<AssetBrowser>();
	RegisterUIComponent<Inspector>();
	RegisterUIComponent<SceneManager>();
	RegisterUIComponent<SpawnWindow>();
	RegisterUIComponent<LevelManager>();
	RegisterUIComponent<Console>();

	m_SceneManager = GetUIComponent<SceneManager>();

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

	LoadEditorSettings();

	//TODO:
	//if (m_Settings.m_StartingLevel.length() > 0)
	//{
	//	World::Get()->LoadLevel(m_Settings.m_StartingLevel);
	//}
	World::Get()->LoadLevel();
}

void Editor::OnDetach()
{
}

void Editor::OnImGuiRender(bool DrawEditor)
{
	//Draw the menu bar first to avoid it being included in the DockSpace.
	ImVec2 Offset = m_MenuBar->Draw();

	InitialiseDockspace(Offset);

	if (DrawEditor)
	{
		RenderApplicationDebug(m_FrameDeltaTime);
		UpdateCollisionEditor();

		//m_Console.Draw();

		//Draw configuration UI
		for (auto& tool : m_Tools)
		{
			if (tool->IsConfigWindowOpen())
			{
				tool->DrawConfigWindow();
			}
		}

		// Draw UI Components
		for (auto& uiComponent : m_UIComponents)
		{
			uiComponent->Render();
		}

		if (m_DrawDemoWindow)
		{
			ImGui::ShowDemoWindow(&m_DrawDemoWindow);
		}		

		if (m_ShowSettingsWindow)
		{
			m_SettingsWindow.Render(m_Settings, *this);
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
	{
		m_EditorCam = dynamic_cast<EditorCamera*>(RenderCommand::GetMainCamera());
	}

	UpdateTools(DeltaTime);
	RenderTools();
	
	for (UIComponent* uiComponent : m_UIComponents)
	{
		uiComponent->Update();
	}
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

void Editor::SaveEditorSettings()
{
	PROFILE_FUNCTION();

	fs::path SettingsPath = Application::GetEnginePath() / "Editor/Settings.txt";
	std::ofstream OutStream = std::ofstream(SettingsPath, std::ios::out | std::ios::trunc);

	if (OutStream.is_open() == false)
	{
		FLOW_ENGINE_ERROR("Editor::SaveEditorSettings: Failed to save settings %s", SettingsPath.string().c_str());
		return;
	}

	// Starting level name
	size_t size = m_Settings.m_StartingLevel.length();
	OutStream << size;
	//OutStream << m_Settings.m_StartingLevel;

	//OutStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	OutStream.write(m_Settings.m_StartingLevel.c_str(), sizeof(char) * size);

	// Colors
	OutStream.write(reinterpret_cast<const char*>(&m_Settings.m_ObjectHighlightColour), sizeof(Vector3));

	OutStream.close();
}

void Editor::LoadEditorSettings()
{
	PROFILE_FUNCTION();

	std::ifstream InputStream = std::ifstream(Application::GetEnginePath() / "Editor/Settings.txt", std::ios::in);

	if (InputStream.is_open() == false)
	{
		FLOW_ENGINE_ERROR("Editor::LoadEditorSettings: Failed to editor settings");
		return;
	}

	// Starting level name
	size_t size;
	char buffer[128] = { '\0' };

	InputStream >> size;
	//InputStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	InputStream.read(buffer, sizeof(char) * size);
	m_Settings.m_StartingLevel = buffer;

	// Colors
	InputStream.read(reinterpret_cast<char*>(&m_Settings.m_ObjectHighlightColour), sizeof(Vector3));

	InputStream.close();
}

Editor& Editor::Get()
{
	return *Application::Get().GetEditor();
}

Editor::Settings& Editor::GetEditorSettings()
{
	return Editor::Get().m_Settings;
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

bool Editor::IsSceneWindowFocused() const
{
	return m_SceneManager->IsSceneWindowFocused();
}

bool Editor::IsMouseOverScene() const
{
	return m_SceneManager->IsMouseOverScene();
}

IntVector2 Editor::GetSceneWindowSize() const
{
	return m_SceneManager->GetSceneWindowSize();
}

IntVector2 Editor::GetSceneWindowPosition() const
{
	return m_SceneManager->GetSceneWindowPosition();
}

bool Editor::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	bool Handled = false;

	CONSUMES_INPUT(m_EditorCam, m_EditorCam->OnMouseButtonPressed(e));

	return false;
}

bool Editor::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
{
	CONSUMES_INPUT(m_EditorCam, m_EditorCam->OnMouseButtonReleased(e));

	return false;
}

bool Editor::OnMouseMoved(MouseMovedEvent& e)
{
	return false;
}

bool Editor::OnMouseScrolled(MouseScrolledEvent& e)
{
	CONSUMES_INPUT(m_EditorCam, m_EditorCam->OnMouseScrolled(e));

	return false;
}

bool Editor::OnKeyPressed(KeyPressedEvent& e)
{
	PASS_KEY_EVENT(m_EditorCam);

	for (Tool* tool : m_Tools)
	{
		PASS_KEY_EVENT(tool);
	}

	for (UIComponent* component : m_UIComponents)
	{
		PASS_KEY_EVENT(component);
	}

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
		if (ImGui::BeginChild("Pass Config", ImVec2(0, 0), true))
		{			
			float freeSpace = ImGui::GetContentRegionAvailWidth() - ImGui::CalcTextSize("Enabled Render Passes").x;
			float centreOffset = freeSpace / 2.0f;
			ImGui::SetCursorPosX(centreOffset);
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Enabled Render Passes");

			RenderQueue* Queue = RenderQueue::Get();
			ImGui::Checkbox("Main Pass (Back Culled)", &Queue->m_Pass0Enabled);
			ImGui::Checkbox("Main Pass (Forward Culled)", &Queue->m_Pass1Enabled);
			ImGui::Checkbox("Main Pass (Two Sided)", &Queue->m_Pass2Enabled);
			ImGui::Checkbox("Mask Pass", &Queue->m_Pass3Enabled);
			ImGui::Checkbox("Outline Pass", &Queue->m_Pass4Enabled);
			ImGui::Checkbox("No Depth Pass", &Queue->m_Pass5Enabled);
			ImGui::Checkbox("2D Pass", &Queue->m_Pass6Enabled);
			ImGui::Checkbox("UI Pass", &Queue->m_Pass7Enabled);
		}
		ImGui::EndChild();

	}
	ImGui::End();
}

//= Settings Window ======================================================

void Editor::SettingsWindow::Render(Editor::Settings& EditorSettings, Editor& EditorRef)
{
	if (EditorRef.m_ShowSettingsWindow == false)
	{
		return;
	}

	if (ImGui::Begin("Editor Settings", &EditorRef.m_ShowSettingsWindow))
	{
		ImGui::Text("Colors");
		ImGui::ColorEdit3("Selected Object Highlight Colour", reinterpret_cast<float*>(&EditorSettings.m_ObjectHighlightColour));

		ImGui::Separator();

		ImGui::Text("Levels");
		if (ImGui::InputText("Starting Level Name", m_StartingNameBuffer, 128, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			EditorSettings.m_StartingLevel = m_StartingNameBuffer;
		}

		ImGui::Text("Appearance");
		ImGui::Checkbox("Enable docked scene tools", &EditorSettings.m_DockFloatingWindows);


		if (ImGui::Button("Save and Close"))
		{
			//Grab settings from buffers
			EditorSettings.m_StartingLevel = m_StartingNameBuffer;

			//Save
			EditorRef.SaveEditorSettings();
			EditorRef.m_ShowSettingsWindow = false;
		}
	}
	ImGui::End();
}
