#pragma once

//= Includes ==============================================

#include <string>

//= Class Definition ======================================

class LevelManager
{
public:

	//= Public Functions ==================================

					LevelManager();
	void			Open_NewLevelWindow();
	void			DrawWindows();


private:

	//= Private Functions ==================================

	void			Draw_NewLevelWindow();


private:

	//= Private Variables ==================================

	bool			m_Draw_NewLevelWindow;
	std::string		m_NewLevelName;
};