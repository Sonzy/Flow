#pragma once

class EditorLayer;
struct ImVec2;

class MenuBar
{
public:

	MenuBar(EditorLayer* EditorPointer);
	ImVec2 Draw();



private:
	EditorLayer* _Editor;
	bool _Visible_BulletConfiguration;
	bool _Visible_EditorSettings;

private:

	//= Windows ========

	void Window_BulletDebugDrawSettings();
	void Window_EditorSettings();
};