#pragma once

class EditorLayer;
struct ImVec2;

class Toolbar
{
public:

	Toolbar(EditorLayer* EditorPointer);
	ImVec2 Draw();

	void DrawBulletColourConfig();

private:
	EditorLayer* _Editor;

	bool _ShowBulletConfigurationWindow;
};