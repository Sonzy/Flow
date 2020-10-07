#include "Flowpch.h"
#include "SceneManager.h"

#include "ThirdParty/ImGui/imgui.h"

#include "Flow/GameFramework/World.h"

#include "Flow/Editor/EditorCamera.h"

#include "Flow/Rendering/Other/FrameBuffer.h"

#include "Toolbar.h"

SceneManager::SceneManager()
	:
	m_CachedWindowSize(0, 0),
	m_SceneWindowPosition(0, 0),
	m_SceneWindowSize(0, 0),
	m_SceneFocused(false),
	m_MouseOverScene(false),
	m_EditorCam(nullptr)
{
	m_Toolbar = new ToolBar();
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update(float DeltaTime)
{
	if (!m_EditorCam)
		m_EditorCam = static_cast<EditorCamera*>(RenderCommand::GetMainCamera().get());
}

void SceneManager::DrawWindow_Scene()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	auto State = World::Get()->GetWorldState();
	std::string WindowName;
	switch (State)
	{
	case WorldState::Paused: WindowName = "Scene - Paused###Scene"; break;
	case WorldState::Editor: WindowName = "Scene###Scene"; break;
	case WorldState::InGame: WindowName = "Scene - Playing###Scene"; break;
	}

	if (ImGui::Begin(WindowName.c_str()))
	{
		m_Toolbar->DrawWindow();

		m_SceneFocused = ImGui::IsWindowFocused();

		//TODO: should only push input if the scene is focused, rather than the camera blocking
		if (m_EditorCam)
			m_EditorCam->m_CanUpdate = m_SceneFocused ? true : !ImGui::IsAnyItemActive();

		FrameBuffer* Buff = RenderCommand::GetEditorFrameBuffer();

		//Note: We assume that the scene image has no padding and is flush to the window x. 
		m_SceneWindowPosition = IntVector2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
		m_SceneWindowSize = IntVector2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		m_MouseOverScene = ImGui::IsWindowHovered();

		m_SceneWindowSize.x = std::clamp(m_SceneWindowSize.x, 1, 1920); //TODO: Make max texture size not fixed
		m_SceneWindowSize.y = std::clamp(m_SceneWindowSize.y, 1, 1080); //TODO: Make max texture size not fixed

		if (*reinterpret_cast<Vector2*>(&m_SceneWindowSize) != m_CachedWindowSize)//TODO: Stop being naughty
		{
			Buff->Resize(m_SceneWindowSize.x, m_SceneWindowSize.y);
			m_CachedWindowSize = *reinterpret_cast<Vector2*>(&m_SceneWindowSize);
		}

		ImGui::Image(Buff->GetTextureView(), ImVec2(static_cast<float>(Buff->GetWidth()), static_cast<float>(Buff->GetHeight())));
	}
	ImGui::End();

	ImGui::PopStyleVar();
}
