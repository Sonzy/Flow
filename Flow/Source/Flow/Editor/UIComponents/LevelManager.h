#pragma once

#if WITH_EDITOR

// Includes ///////////////////////////////////////////////////////////////////////

#include <string>

#include "UIComponent.h"

// Class Definition //////////////////////////////////////////////////////////////

class LevelManager : public UIComponent
{
public:

	// Public Functions ////////////////////////////////////////////////////////////////////

					LevelManager();

	//= UI Component Interface =

	virtual void	Update() override;
	virtual void	Render() override;
	   
	void			Open_NewLevelWindow();

private:

	// Private Functions ////////////////////////////////////////////////////////////////////

	//= Window Drawing =
	void			Draw_NewLevel();


private:

	// Private Variables ////////////////////////////////////////////////////////////////////

	bool			m_DrawWindow_NewLevel;
	std::string		m_NewLevelName;
};

#endif // WITH_EDITOR