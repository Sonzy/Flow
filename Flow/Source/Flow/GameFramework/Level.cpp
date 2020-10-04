#include "Flowpch.h"
#include "Level.h"
#include "Actor.h"
#include "Flow/GameFramework/Other/ClassFactory.h"

Level::Level(const std::string& LevelName)
	: m_Name(LevelName)
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
	OutStream.write(reinterpret_cast<const char*>(&m_Name), sizeof(char) * 32);

	//Write level metadata


	//Write the number of actors in the file
	int NumberOfActors = m_Actors.size();
	OutStream.write(reinterpret_cast<const char*>(&NumberOfActors), sizeof(int));

	for (auto Object : m_Actors)
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
	m_Actors.erase(m_Actors.begin(), m_Actors.end());

	std::string SavePath = "Saved/SaveFile.flvl";
	std::ifstream InputStream = std::ifstream(SavePath, std::ios::in | std::ios::binary);
	if (!InputStream)
	{
		FLOW_ENGINE_ERROR("Failed to load save file at path: {0}", SavePath);
		return;
	}

	//Load Level Name
	InputStream.read(reinterpret_cast<char*>(&m_Name), sizeof(char) * 32);

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
		m_Actors.push_back(NewActor);
	}
}

void Level::InitialiseTickList()
{
	for (auto& BeginActor : m_Actors)
	{
		if(BeginActor->IsTickEnabled())
			m_TickList.push_back(BeginActor);
	}
}

void Level::DispatchBeginPlay()
{
	for (auto& BeginActor : m_Actors)
	{
		BeginActor->BeginPlay();
	}
}

void Level::DispatchEditorBeginPlay()
{
	for (auto& BeginActor : m_Actors)
	{
		BeginActor->EditorBeginPlay();
	}
}

void Level::Tick(float DeltaTime)
{
	for (auto& TickActor : m_TickList)
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
		m_TickList.push_back(TickActor);
		TickActor->m_TickEnabled = true;
	}
	else
	{
		m_TickList.erase(std::find(m_TickList.begin(), m_TickList.end(), TickActor));
		TickActor->m_TickEnabled = false;
	}
}
