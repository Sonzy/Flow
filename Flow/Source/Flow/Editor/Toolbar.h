#pragma once
#include "Flow/Rendering/Core/Bindables/Texture.h"

class ToolBar
{
public:

	ToolBar();
	~ToolBar();
	ToolBar(ToolBar&) = delete;
	ToolBar(ToolBar&&) = delete;

	void DrawWindow();

	Texture* m_Icon_SelectionTool;

	Texture* m_Icon_Play;
	Texture* m_Icon_Pause;
	Texture* m_Icon_Stop;
};