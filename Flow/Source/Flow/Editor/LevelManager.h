#pragma once
#include <string>

class LevelManager
{
public:
	LevelManager();

	//Windows
	void Open_NewLevelWindow();

	void DrawWindows();


private:

	//Window Drawing
	void Draw_NewLevelWindow();


private:

	//Window Drawing
	bool _Draw_NewLevelWindow;



	//NewLevelWindow
	std::string _NewLevelName;
};