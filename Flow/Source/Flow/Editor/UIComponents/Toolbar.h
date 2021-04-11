#pragma once

#if WITH_EDITOR

// Includes //////////////////////////////////

#include "UIComponent.h"

namespace Bindables
{
	class Texture;
}

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

	Bindables::Texture*				m_Icon_SelectionTool;

	Bindables::Texture*				m_Icon_Play;
	Bindables::Texture*				m_Icon_Pause;
	Bindables::Texture*				m_Icon_Stop;
};

#endif // WITH_EDITOR