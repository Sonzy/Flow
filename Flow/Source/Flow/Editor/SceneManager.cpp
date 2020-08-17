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

	tb = new ToolBar();
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
		tb->DrawWindow();

		m_SceneFocused = ImGui::IsWindowFocused();

		//TODO: should only push input if the scene is focused, rather than the camera blocking
		if (m_EditorCam)
			m_EditorCam->_CanUpdate = m_SceneFocused ? true : !ImGui::IsAnyItemActive();

		FrameBuffer* Buff = RenderCommand::GetEditorFrameBuffer();

		//Note: We assume that the scene image has no padding and is flush to the window x. 
		m_SceneWindowPosition = IntVector2D(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
		m_SceneWindowSize = IntVector2D(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		m_MouseOverScene = ImGui::IsWindowHovered();

		m_SceneWindowSize.X = std::clamp(m_SceneWindowSize.X, 1, 1920); //TODO: Make max texture size not fixed
		m_SceneWindowSize.Y = std::clamp(m_SceneWindowSize.Y, 1, 1080); //TODO: Make max texture size not fixed

		if (*reinterpret_cast<Vector2D*>(&m_SceneWindowSize) != m_CachedWindowSize)//TODO: Stop being naughty
		{
			Buff->Resize(m_SceneWindowSize.X, m_SceneWindowSize.Y);
			m_CachedWindowSize = *reinterpret_cast<Vector2D*>(&m_SceneWindowSize);
		}

		ImGui::Image(Buff->GetTextureView(), ImVec2(Buff->GetWidth(), Buff->GetHeight()));
	}
	ImGui::End();

	ImGui::PopStyleVar();
}
