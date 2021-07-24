#pragma once

//= Includes =================================

#include <string>
#include "Typedefs.h"

//= Forward Declarations =====================

class Actor;
namespace YAML { class Emitter; }
namespace YAML { class Node; }

//= Class Definition =========================

class Level
{
public:

	//= Public Functions =============

											Level() = delete;
											Level(const std::string& LevelName);

	void									Save(YAML::Emitter& file);
	bool									Load(YAML::Node& Input);

	void									InitialiseTickList();
	void									DispatchBeginPlay();
#if WITH_EDITOR
	void									DispatchEditorBeginPlay();
#endif
	void									Tick(float DeltaTime);
	void									SetTickEnabled(Actor* Actor, bool Enable);

private:

	//= Private Variables ==========

	std::string								m_Name;
	std::unordered_map<FGuid, Actor*>		m_Actors;
	std::vector<Actor*>						m_TickList;
};