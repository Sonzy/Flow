#pragma once

//= Includes =================================

#include <string>

//= Forward Declarations =====================

class Actor;

//= Class Definition =========================

class Level
{
public:

	//= Public Functions =============

											Level() = delete;
											Level(const std::string& LevelName);

	void									Save(std::ofstream& Output);
	void									Load(std::ifstream& Input);

	void									InitialiseTickList();
	void									DispatchBeginPlay();
#if WITH_EDITOR
	void									DispatchEditorBeginPlay();
#endif
	void									Tick(float DeltaTime);
	void									SetTickEnabled(Actor* Actor, bool Enable);

	std::vector<Actor*>&	GetActors()		{ return m_Actors; }

private:

	//= Private Variables ==========

	std::string								m_Name;
	std::vector<Actor*>						m_Actors;
	std::vector<Actor*>						m_TickList;
};