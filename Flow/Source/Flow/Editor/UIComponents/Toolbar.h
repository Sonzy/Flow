#pragma once

// Includes //////////////////////////////////

#include "UIComponent.h"
#include "Flow/Rendering/Core/Bindables/Texture.h"

// Class Definition //////////////////////////////////

// Editor toolbar that contains the playstate controls for the scene window.
class ToolBar : public UIComponent
{
public:

	// Public Function //////////////////////////////////

							ToolBar();
							~ToolBar();
							ToolBar(ToolBar&) = delete;
							ToolBar(ToolBar&&) = delete;

	virtual void			Update() override;
	virtual void			Render() override;

	// Public Variables /////////////////////////////////

	Texture*				m_Icon_SelectionTool;

	Texture*				m_Icon_Play;
	Texture*				m_Icon_Pause;
	Texture*				m_Icon_Stop;
};