#include "Flowpch.h"
#include "Editor.h"
#include "Events/MouseEvent.h"
#include "Editor/UIComponents/Inspector.h"
#include "Application.h"

#include "GameFramework/World.h"
#include "Editor/MenuBar.h"
#include "Editor/UIComponents/AssetBrowser.h"

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_internal.h"

#include <Psapi.h> //memory debug
#include "Flow/Rendering/Core/RenderQueue/RenderQueue.h"

#include "Rendering/Other/FrameBuffer.h"
#include "Editor/EditorCamera.h"
#include "Editor/UIComponents/LevelManager.h"
#include "Editor/UIComponents/SpawnWindow.h"
#include "Editor/UIComponents/Console.h"
#include "Editor/IconManager.h"

#include "Editor/UIComponents/Toolbar.h"
#include "Editor/Tools/Tool.h"
#include "Editor/Tools/SelectionTool.h"

#include <yaml-cpp/yaml.h>
#include "Utils/YamlSerializer.h"

#define PASS_KEY_EVENT(MemberName) 	if (MemberName != nullptr && MemberName->OnKeyPressed(e)) { return true; }

Editor::Editor()
	: Layer("Editor Layer")
	, m_EditorViewportSize(0,0)
	, m_Initialised(false)
	, m_ShowSettingsWindow(false)
	, m_ShowPrototypingWindow(false)
	, m_DrawDemoWindow(false)
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
	RegisterUIComponent<IconManager>();

	m_SceneManager = GetUIComponent<SceneManager>();

	m_Initialised = true;
}

void Editor::InitialiseEditor()
{
	World::Get()->LoadLevel();
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

		// Draw tools
		for (Tool* tool : m_Tools)
		{
			tool->RenderToolImGui();
		}

		if (m_DrawDemoWindow)
		{
			ImGui::ShowDemoWindow(&m_DrawDemoWindow);
		}		

		if (m_ShowSettingsWindow)
		{
			m_SettingsWindow.Render(m_Settings, *this);
		}

		if (m_ShowPrototypingWindow)
		{
			ImGuiPrototypingWindow();
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

	YAML::Emitter file;

	file << YAML::BeginMap;
	{
		file << YAML::Key << "Levels";
		file << YAML::BeginMap;
		{
			file << YAML::Key << "StartingLevelName";
			file << YAML::Value << m_Settings.m_StartingLevel;
		}
		file << YAML::EndMap;

		file << YAML::Key << "Docking";
		file << YAML::BeginMap;
		{
			file << YAML::Key << "DockFloatingWindows";
			file << YAML::Value << m_Settings.m_DockFloatingWindows;

			file << YAML::Key << "DockPadding";
			file << YAML::Value << m_Settings.m_DockPadding;
		}
		file << YAML::EndMap;

		file << YAML::Key << "Console";
		file << YAML::BeginMap;
		{
			file << YAML::Key << "LogColor";
			file << YAML::Value << m_Settings.m_consoleLogColor;

			file << YAML::Key << "WarningColor";
			file << YAML::Value << m_Settings.m_consoleWarningColor;

			file << YAML::Key << "ErrorColor";
			file << YAML::Value << m_Settings.m_consoleErrorColor;
		}
		file << YAML::EndMap;

		file << YAML::Key << "EditorCamera";
		file << YAML::BeginMap;
		{
			file << YAML::Key << "Speed";
			file << YAML::Value << m_Settings.m_cameraSpeed;

			file << YAML::Key << "PanningSpeed";
			file << YAML::Value << m_Settings.m_cameraPanningSpeed;

			file << YAML::Key << "ScrollingSpeed";
			file << YAML::Value << m_Settings.m_cameraScrollingSpeed;
		}
		file << YAML::EndMap;

		file << YAML::Key << "Other";
		file << YAML::BeginMap;
		{
			file << YAML::Key << "ObjectHighlightColor";
			file << YAML::Value << m_Settings.m_ObjectHighlightColour;
		}
		file << YAML::EndMap;

		file << YAML::Key << "Testing";
		file << YAML::BeginMap;
		{
			file << YAML::Key << "ShowImGuiPrototypingWindow";
			file << YAML::Value << m_ShowPrototypingWindow;
		}
		file << YAML::EndMap;

		file << YAML::Key << "Rendering";
		file << YAML::BeginMap;
		{
			file << YAML::Key << "NearPlane";
			file << YAML::Value << RenderCommand::GetNearPlaneRef();

			file << YAML::Key << "FarPlane";
			file << YAML::Value << RenderCommand::GetFarPlaneRef();
		}
		file << YAML::EndMap;
	}
	file << YAML::EndMap;

	fs::path savePath = Application::GetEnginePath() / "Editor/Settings.yaml"; //.EngineSettings
	std::ofstream OutStream = std::ofstream(savePath);
	OutStream << file.c_str();
	OutStream.close();
}

void Editor::LoadEditorSettings()
{
	PROFILE_FUNCTION();

	fs::path savePath = Application::GetEnginePath() / "Editor/Settings.yaml"; //.EngineSettings
	std::ifstream InStream = std::ifstream(savePath);
	if (InStream.is_open() == false)
	{
		return;
	}

	std::stringstream stream;
	stream << InStream.rdbuf();

	YAML::Node data = YAML::Load(stream.str());

	YAML::Node levelData = data["Levels"];
	{
		m_Settings.m_StartingLevel = levelData["StartingLevelName"].as<std::string>();
	}

	YAML::Node dockingData = data["Docking"];
	{
		m_Settings.m_DockFloatingWindows = dockingData["DockFloatingWindows"].as<bool>();
		m_Settings.m_DockPadding = dockingData["DockPadding"].as<Vector2>();
	}

	YAML::Node consoleData = data["Console"];
	{
		m_Settings.m_consoleLogColor = consoleData["LogColor"].as<Vector4>();
		m_Settings.m_consoleWarningColor = consoleData["WarningColor"].as<Vector4>();
		m_Settings.m_consoleErrorColor = consoleData["ErrorColor"].as<Vector4>();
	}

	YAML::Node editorCameraData = data["EditorCamera"];
	{
		m_Settings.m_cameraSpeed = editorCameraData["Speed"].as<float>();
		m_Settings.m_cameraPanningSpeed = editorCameraData["PanningSpeed"].as<float>();
		m_Settings.m_cameraScrollingSpeed = editorCameraData["ScrollingSpeed"].as<float>();
	}

	YAML::Node otherData = data["Other"];
	{
		m_Settings.m_ObjectHighlightColour = otherData["ObjectHighlightColor"].as<Vector3>();
	}

	if (YAML::Node testingData = data["Testing"])
	{
		m_ShowPrototypingWindow = testingData["ShowImGuiPrototypingWindow"].as<bool>();
	}

	if (YAML::Node renderingData = data["Rendering"])
	{
		RenderCommand::SetNearPlane(renderingData["NearPlane"].as<float>());
		RenderCommand::SetFarPlane(renderingData["FarPlane"].as<float>());
	}

	InStream.close();
}

Editor& Editor::Get()
{
	return *Application::Get().GetEditor();
}

Editor::Settings& Editor::GetSettings()
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

void Editor::ImGuiPrototypingWindow()
{
	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;
	if (ImGui::Begin("Prototyping Window", &m_ShowPrototypingWindow, windowFlags))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Search Filters"))
			{
				static bool m_filterImapsWithinDistance;
				static float m_filterImapsWithinDistancefloat;

				// Menu item with inline 
				const char* text = "Hide IMAPs outside of distance";
				ImVec2 size = ImGui::CalcTextSize(text);
				size.y = 0;
				if (ImGui::Selectable(text, false, 0, size))
				{
					m_filterImapsWithinDistance = !m_filterImapsWithinDistance;
				}

				ImGui::SameLine();
				ImVec2 offset = ImVec2(ImGui::GetCursorPosX() + 30.0f, ImGui::GetCursorPosY());
				float xPos = ImGui::GetCursorPosX();
				if (m_filterImapsWithinDistance == true)
				{					
					ImVec2 position = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPosX() + 5.0f, ImGui::GetWindowPos().y + ImGui::GetCursorPosY());
					ImGui::RenderCheckMark(position, ImGui::GetColorU32(ImGuiCol_Text), ImGui::GetFontSize() * 0.866f);
				}

				float inputSize = 80.0f;
				float remainingWidth = ImGui::GetContentRegionAvailWidth() - offset.x;
				if (remainingWidth < inputSize)
				{
					ImGui::SetCursorPosX(xPos + 30.0f);			
				}
												
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
				ImGui::SameLine(offset.x); ImGui::PushItemWidth(inputSize);
				ImGui::InputFloat("", &m_filterImapsWithinDistancefloat,0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
				ImGui::PopItemWidth();
				ImGui::PopStyleVar();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImVec2 position = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x + ImGui::GetCursorPosX(), ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y + ImGui::GetCursorPosY());
		ImGui::RenderCheckMark(position, ImGui::GetColorU32(ImGuiCol_Text), ImGui::GetFontSize() * 0.866f);

		ImGui::GetCurrentContext()->NavDisableMouseHover = true;
		static bool selected = false;
		ImGui::Selectable(selected ? "True" : "False", &selected);
		ImGui::GetCurrentContext()->NavDisableMouseHover = false;

		static bool selected2 = false;
		ImGui::Selectable("Test2", &selected2);
	}
	ImGui::End();
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

		ImGui::Text("Viewport Size: %d, %d", m_SceneManager->GetSceneWindowSize().x, m_SceneManager->GetSceneWindowSize().y);
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

			ImGui::Checkbox("Main", &RenderQueue::GetPass(RenderPass::Main).Enabled());
			ImGui::Checkbox("Outline Masking", &RenderQueue::GetPass(RenderPass::OutlineMasking).Enabled());
			ImGui::Checkbox("Outline", &RenderQueue::GetPass(RenderPass::Outline).Enabled());
			ImGui::Checkbox("No Depth", &RenderQueue::GetPass(RenderPass::NoDepth).Enabled());
			ImGui::Checkbox("2D", &RenderQueue::GetPass(RenderPass::Standard2D).Enabled());
			ImGui::Checkbox("UI", &RenderQueue::GetPass(RenderPass::UI).Enabled());
			ImGui::Checkbox("Selection", &RenderQueue::GetPass(RenderPass::Selection).Enabled());
			ImGui::Checkbox("Front Culling", &RenderQueue::GetPass(RenderPass::FrontFaceCulling).Enabled());

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
		bool updateFile = false;

		ImGui::Text("Levels");
		{
			if (ImGui::InputText("Starting Level Name", m_StartingNameBuffer, 128, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				updateFile = true;
				EditorSettings.m_StartingLevel = m_StartingNameBuffer;
			}
		}
		ImGui::Separator();
		ImGui::NewLine();

		ImGui::Text("Docking");
		{
			updateFile |= ImGui::Checkbox("Enable docked scene tools", &EditorSettings.m_DockFloatingWindows);
			updateFile |= ImGui::InputFloat2("Docking padding", EditorSettings.m_DockPadding.Data());
		}
		ImGui::Separator();
		ImGui::NewLine();

		ImGui::Text("Console");
		{
			updateFile |= ImGui::ColorEdit4("Log Color", EditorSettings.m_consoleLogColor.Data());
			updateFile |= ImGui::ColorEdit4("Warning Color", EditorSettings.m_consoleWarningColor.Data());
			updateFile |= ImGui::ColorEdit4("Error Color", EditorSettings.m_consoleErrorColor.Data());
		}
		ImGui::Separator();
		ImGui::NewLine();

		ImGui::Text("Editor Camera");
		{
			updateFile |= ImGui::InputFloat("Speed", &EditorSettings.m_cameraSpeed);
			updateFile |= ImGui::InputFloat("Panning Speed", &EditorSettings.m_cameraPanningSpeed);
			updateFile |= ImGui::InputFloat("Scrolling Speed", &EditorSettings.m_cameraScrollingSpeed);
		}
		ImGui::Separator();
		ImGui::NewLine();

		ImGui::Text("Other");
		{
			updateFile |= ImGui::ColorEdit3("Selected Object Highlight Colour", reinterpret_cast<float*>(&EditorSettings.m_ObjectHighlightColour));
		}
		ImGui::Separator();

		ImGui::Text("Testing");
		{
			updateFile |= ImGui::Checkbox("Show ImGui Prototyping Window", &EditorRef.m_ShowPrototypingWindow);
		}
		ImGui::Separator();

		ImGui::Text("Rendering"); //TODO: Move somewhere else
		{
			bool updatedRenderSettings = false;
			if (ImGui::InputFloat("Near Plane", &RenderCommand::GetNearPlaneRef(), 0.05f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				float& nearPlane = RenderCommand::GetNearPlaneRef();
				if (nearPlane <= 0.0f)
				{
					nearPlane = 0.05f;
				}

				updatedRenderSettings = true;
			}

			if (ImGui::InputFloat("Far Plane", &RenderCommand::GetFarPlaneRef(), 10.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				updatedRenderSettings = true;
			}

			const char* sceneBufferOptions[] = { "Game", "Selection" };
			static int currentSceneBufferOption = 0;
			if (ImGui::Combo("Output Buffer", &currentSceneBufferOption, sceneBufferOptions, IM_ARRAYSIZE(sceneBufferOptions)))
			{
				updatedRenderSettings = true;
				Editor::Get().GetUIComponent<SceneManager>()->SetViewMode(static_cast<SceneBufferView>(currentSceneBufferOption));
			}

			updateFile |= updatedRenderSettings;
		}
		ImGui::Separator();


		ImGui::Text("Editor Appearance");
		{
			const char* themes[] = { "Unity", "Slate", "RayTeak" };
			static int currentTheme = 0;
			if (ImGui::Combo("Current Theme", &currentTheme, themes, IM_ARRAYSIZE(themes)))
			{
				updateFile = true;
				ImGuiLayer::SetTheme((ImGuiTheme)currentTheme);
			}
		}
		ImGui::Separator();

		if (updateFile == true)
		{
			EditorRef.SaveEditorSettings();
		}
	}
	ImGui::End();
}
