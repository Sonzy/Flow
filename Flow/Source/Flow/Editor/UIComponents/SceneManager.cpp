#include "Flowpch.h"
#include "SceneManager.h"

#include "GameFramework/World.h"
#include "Editor/EditorCamera.h"
#include "Editor/Editor.h"
#include "Editor/Tools/SelectionTool.h"
#include "Rendering/RenderCommand.h"
#include "Rendering/Other/FrameBuffer.h"
#include "Toolbar.h"

#include "ThirdParty/ImGui/imgui.h"

SceneManager::SceneManager()
	: m_CachedWindowSize(0, 0)
	, m_SceneWindowPosition(0, 0)
	, m_SceneWindowSize(0, 0)
	, m_SceneFocused(false)
	, m_MouseOverScene(false)
	, m_EditorCam(nullptr)
	, m_Toolbar(new ToolBar())
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update()
{
	if (m_EditorCam == nullptr)
	{
		m_EditorCam = static_cast<EditorCamera*>(RenderCommand::GetMainCamera());
	}
}

void SceneManager::Render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	auto State = World::Get().GetWorldState();
	std::string WindowName;
	switch (State)
	{
	case WorldState::Paused: WindowName = "Scene - Paused###Scene";		break;
	case WorldState::Editor: WindowName = "Scene###Scene";				break;
	case WorldState::InGame: WindowName = "Scene - Playing###Scene";	break;
	}

	if (ImGui::Begin(WindowName.c_str()))
	{
		m_Toolbar->Render();

		m_SceneFocused = ImGui::IsWindowFocused();

		//TODO: should only push input if the scene is focused, rather than the camera blocking
		if (m_EditorCam)
		{
			m_EditorCam->m_CanUpdate = m_SceneFocused ? true : !ImGui::IsAnyItemActive();
		}


		FrameBuffer* Buff = RenderCommand::GetEditorFrameBuffer();;
		const FrameBuffer* OutputBuffer = nullptr;
		switch (m_viewMode)
		{
		case SceneBufferView::Game:
			OutputBuffer = RenderCommand::GetEditorFrameBuffer();
			break;
		case SceneBufferView::Selection:
			OutputBuffer = RenderQueue::GetSelectionBuffer();
			break;
		default:
			OutputBuffer = RenderCommand::GetEditorFrameBuffer();
			break;
		}

		//Note: We assume that the scene image has no padding and is flush to the window x. 
		m_SceneWindowPosition = IntVector2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
		m_SceneWindowSize = IntVector2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		m_MouseOverScene = ImGui::IsWindowHovered();

		IntVector2 WindowSize = RenderCommand::GetWindowSize();
		m_SceneWindowSize.x = std::clamp(m_SceneWindowSize.x, 1, WindowSize.x);
		m_SceneWindowSize.y = std::clamp(m_SceneWindowSize.y, 1, WindowSize.y);

		if (*reinterpret_cast<Vector2*>(&m_SceneWindowSize) != m_CachedWindowSize)
		{
			Buff->Resize(m_SceneWindowSize.x, m_SceneWindowSize.y);
			m_CachedWindowSize = *reinterpret_cast<Vector2*>(&m_SceneWindowSize);
		}

		ImGui::Image(OutputBuffer->GetTextureView(), ImVec2(static_cast<float>(Buff->GetWidth()), static_cast<float>(Buff->GetHeight())));

		Editor::Get().GetTool<SelectionTool>()->RenderImGuiGizmo();
	}
	ImGui::End();

	ImGui::PopStyleVar();
}

void SceneManager::SetViewMode(SceneBufferView view)
{
	m_viewMode = view;
}
