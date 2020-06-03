#include "Flowpch.h"
#include "Level.h"
#include "Actor.h"

Level::Level(const std::string& LevelName)
	: _Name(LevelName)
{
}

void Level::InitialiseTickList()
{
	for (auto& BeginActor : _Actors)
	{
		if(BeginActor->IsTickEnabled())
			_TickList.push_back(BeginActor);
	}
}

void Level::DispatchBeginPlay()
{
	for (auto& BeginActor : _Actors)
	{
		BeginActor->BeginPlay();
	}
}

void Level::DispatchEditorBeginPlay()
{
	for (auto& BeginActor : _Actors)
	{
		BeginActor->EditorBeginPlay();
	}
}

void Level::Tick(float DeltaTime)
{
	for (auto& TickActor : _TickList)
	{
		TickActor->Tick(DeltaTime);
	}
}

void Level::SetTickEnabled(std::shared_ptr<Actor> TickActor, bool Enable)
{
	if (!TickActor)
	{
		FLOW_ENGINE_ERROR("Level::SetTickEnabled: Actor was nullptr");
		return;
	}

	if (TickActor->IsTickEnabled() == Enable)
	{
		FLOW_ENGINE_WARNING("Level::SetTickEnabled: Tick is already correctly set on actor {0}", TickActor->GetName());
		return;
	}

	if (Enable)
	{
		_TickList.push_back(TickActor);
		TickActor->_TickEnabled = true;
	}
	else
	{
		_TickList.erase(std::find(_TickList.begin(), _TickList.end(), TickActor));
		TickActor->_TickEnabled = false;
	}
}
