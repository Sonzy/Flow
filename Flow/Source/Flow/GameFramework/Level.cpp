#include "Flowpch.h"
#include "Level.h"
#include "Actor.h"
#include "Flow/GameFramework/Other/ClassFactory.h"

Level::Level(const std::string& LevelName)
	: _Name(LevelName)
{
}

void Level::Save(std::ofstream& Output)
{
	PROFILE_FUNCTION();

	std::ofstream OutStream = std::ofstream("Saved/SaveFile.flvl", std::ios::out | std::ios::trunc | std::ios::binary);
	if (!OutStream)
	{
		FLOW_ENGINE_LOG("Failed to create save file");
		return;
	}

	//Save Level Name (TODO: Max character length - 32 for now)
	OutStream.write(reinterpret_cast<const char*>(&_Name), sizeof(char) * 32);

	//Write level metadata


	//Write the number of actors in the file
	int NumberOfActors = _Actors.size();
	OutStream.write(reinterpret_cast<const char*>(&NumberOfActors), sizeof(int));

	for (auto Object : _Actors)
	{
		Object->Serialize(&OutStream);
	}

	OutStream.close();

}

void Level::Load(std::ifstream& Input)
{
	//TODO: Should probably just load the entire file in one go and then work on the buffered file

	PROFILE_FUNCTION();

	//TODO: Delete all actors in level
	_Actors.erase(_Actors.begin(), _Actors.end());

	std::string SavePath = "Saved/SaveFile.flvl";
	std::ifstream InputStream = std::ifstream(SavePath, std::ios::in | std::ios::binary);
	if (!InputStream)
	{
		FLOW_ENGINE_ERROR("Failed to load save file at path: {0}", SavePath);
		return;
	}

	//Load Level Name
	InputStream.read(reinterpret_cast<char*>(&_Name), sizeof(char) * 32);

	//Load Level Meta Data

	//Get Number of actors
	int ActorNumber = -1;
	InputStream.read(reinterpret_cast<char*>(&ActorNumber), sizeof(int));

	//Load actor data
	if (ActorNumber < 0)
	{
		FLOW_ENGINE_ERROR("Failed to load actor number: {0}", SavePath);
		return;
	}

	for (int i = 0; i < ActorNumber; i++)
	{
		//Get the UID for the class
		char ActorClassID[32] = "";
		InputStream.read(ActorClassID, sizeof(char) * 32);

		//Spawn an instance of the class
		std::shared_ptr<Actor> NewActor = std::shared_ptr<Actor>(ClassFactory::Get().CreateObjectFromID<Actor>(std::string(ActorClassID)));
		if (!NewActor)
		{
			FLOW_ENGINE_ERROR("Tried to load an actor of class {0} and failed.", ActorClassID);
			continue;
		}
		
		NewActor->Deserialize(&InputStream);
		_Actors.push_back(NewActor);
	}
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
