#pragma once

//= Includes ========================================

#include "UIComponent.h"
#include "Maths/Vector2.h"
#include "Maths/IntVector2.h"

//= Forward Declarations ============================

class EditorCamera;
class ToolBar;
class Texture;

// Enum Definitions /////////////////////////////////

enum class SceneBufferView
{
	Game,
	Selection
};

//= Class Declaration ===============================

class SceneManager : public UIComponent
{
public:

	//= Public Functions ============================

						SceneManager();
						~SceneManager();

						SceneManager(SceneManager&) = delete;
						SceneManager(SceneManager&&) = delete;

	virtual void		Update() override;
	virtual void		Render() override;

	bool				IsSceneWindowFocused() const { return m_SceneFocused; }
	bool				IsMouseOverScene() const { return m_MouseOverScene; }
	IntVector2			GetSceneWindowSize() const { return m_SceneWindowSize; }
	IntVector2			GetSceneWindowPosition() const { return m_SceneWindowPosition; }

	void				SetViewMode(SceneBufferView view);

private:

	//= Private Functions ===========================

	bool				m_SceneFocused;	
	bool				m_MouseOverScene;
	IntVector2			m_SceneWindowSize;
	IntVector2			m_SceneWindowPosition;
	Vector2				m_CachedWindowSize;

	EditorCamera*		m_EditorCam;

	ToolBar*			m_Toolbar;

	SceneBufferView		m_viewMode;
};