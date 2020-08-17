#pragma once
#include "Flow/Helper/Maths.h"

class EditorCamera;

class ToolBar;

class SceneManager
{
public:

	SceneManager();
	~SceneManager();

	SceneManager(SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;

	void Update(float DeltaTime);
	void DrawWindow_Scene();

	bool IsSceneWindowFocused() const { return m_SceneFocused; }
	bool IsMouseOverScene() const { return m_MouseOverScene; }
	IntVector2D GetSceneWindowSize() const { return m_SceneWindowSize; }
	IntVector2D GetSceneWindowPosition() const { return m_SceneWindowPosition; }

private:

	bool m_SceneFocused;	
	bool m_MouseOverScene;
	IntVector2D m_SceneWindowSize;
	IntVector2D m_SceneWindowPosition;
	Vector2D m_CachedWindowSize;

	EditorCamera* m_EditorCam;

	ToolBar* tb;
};