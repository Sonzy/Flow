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
	void									SetTickEnabled(std::shared_ptr<Actor> Actor, bool Enable);

	std::vector<std::shared_ptr<Actor>>&	GetActors() { return m_Actors; }

private:

	//= Private Variables ==========

	std::string								m_Name;
	std::vector<std::shared_ptr<Actor>>		m_Actors;
	std::vector<std::shared_ptr<Actor>>		m_TickList;
};