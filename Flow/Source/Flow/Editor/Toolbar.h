#pragma once

//= Includes ============================================

#include "Flow/Rendering/Core/Bindables/Texture.h"

//= Class Definition ====================================

// Editor toolbar that contains the playstate controls for the scene window.
class ToolBar
{
public:

	//= Public Functions ================================

							ToolBar();
							~ToolBar();
							ToolBar(ToolBar&) = delete;
							ToolBar(ToolBar&&) = delete;

	void					DrawWindow();

	//= Public Variables ================================

	Texture*				m_Icon_SelectionTool;

	Texture*				m_Icon_Play;
	Texture*				m_Icon_Pause;
	Texture*				m_Icon_Stop;
};