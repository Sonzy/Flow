#pragma once
#include "Maths/Vector2.h"
#include "Maths/IntVector2.h"

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
	IntVector2 GetSceneWindowSize() const { return m_SceneWindowSize; }
	IntVector2 GetSceneWindowPosition() const { return m_SceneWindowPosition; }

private:

	bool m_SceneFocused;	
	bool m_MouseOverScene;
	IntVector2 m_SceneWindowSize;
	IntVector2 m_SceneWindowPosition;
	Vector2 m_CachedWindowSize;

	EditorCamera* m_EditorCam;

	ToolBar* tb;
};