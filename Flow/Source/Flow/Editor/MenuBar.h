#pragma once

//= Forward Declarations ===================

class Editor;
struct ImVec2;

//= Class Declarations =====================

// Menu bar drawn at the top of the editor
class MenuBar
{
public:

	//= Public Functions ========================================

					MenuBar(Editor* EditorPointer);

	// Draws the menu bar. Returns the size of the menu so the editor can manage the dockspace
	ImVec2			Draw();

private:

	//= Private Functions ========================================

	void			Window_BulletDebugDrawSettings();
	void			Window_EditorSettings();

private:

	//= Private Variables ========================================

	Editor*			m_Editor;
	bool			m_Visible_BulletConfiguration;
	bool			m_Visible_EditorSettings;


};