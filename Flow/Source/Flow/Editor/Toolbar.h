#pragma once

class EditorLayer;
struct ImVec2;

class Toolbar
{
public:

	Toolbar(EditorLayer* EditorPointer);
	ImVec2 Draw();

private:
	EditorLayer* _Editor;
};