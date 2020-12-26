#include "Flowpch.h"
#include "Level.h"

#include "Flow/GameFramework/Other/ClassFactory.h"
#include "Assets/AssetSystem.h"

#include "Actor.h"
#include "Components/WorldComponent.h"

#include <yaml-cpp/yaml.h>

Level::Level(const std::string& LevelName)
	: m_Name(LevelName)
{
}

void Level::Save(YAML::Emitter& file)
{
	PROFILE_FUNCTION();

	// Generate data
	file << YAML::BeginMap;

	file << YAML::Key << "LevelName";
	file << YAML::Value << m_Name;

	//= Serialise all actors =
	file << YAML::Key << "Actors";
	file << YAML::Value << YAML::BeginSeq;
	for (const std::pair<FGUID, Actor*> actor : World::Get()->GetActorMap())
	{
		file << YAML::BeginMap;
		actor.second->Serialize(file);
		file << YAML::EndMap;
	}
	file << YAML::EndSeq;

	//= Serialise all components =
	file << YAML::Key << "Components";
	file << YAML::Value << YAML::BeginSeq;
	for (const std::pair<FGUID, Component*> component : World::Get()->GetComponentMap())
	{
		file << YAML::BeginMap;
		component.second->Serialize(file);
		file << YAML::EndMap;
	}
	file << YAML::EndSeq;

	file << YAML::EndMap;
}

void Level::Load(YAML::Node& Input)
{
	m_Name = Input["LevelName"].as<std::string>();

	auto actors = Input["Actors"];
	if (actors.IsDefined())
	{
		for (auto actor : actors)
		{
			//Get Class Metadata
			YAML::Node GameObject = actor["GameObject"];
			std::string classID = GameObject["ClassName"].as<std::string>(); //TODO: Set the guid before generating a new one?

			Actor* NewActor = ClassFactory::Get().CreateObjectFromID<Actor>(classID);
			if (NewActor == nullptr)
			{
				FLOW_ENGINE_ERROR("Tried to load an actor of class %s and failed.", classID);
				continue;
			}

			World::Get()->RegisterActor(NewActor);
			NewActor->Deserialize(actor);
		}
	}

	YAML::Node components = Input["Components"];
	if (components.IsDefined())
	{
		for (YAML::iterator::value_type componentNode : components)
		{
			//Get Class Metadata
			YAML::Node GameObject = componentNode["GameObject"];
			std::string classID = GameObject["ClassName"].as<std::string>();

			Component* NewComponent = ClassFactory::Get().CreateObjectFromID<Component>(classID);
			if (NewComponent == nullptr)
			{
				FLOW_ENGINE_ERROR("Tried to load an actor of class %s and failed.", classID);
				continue;
			}

			World::Get()->RegisterComponent(NewComponent);
			NewComponent->Deserialize(componentNode);
		}
	}
}

void Level::InitialiseTickList()
{
	for (std::pair<FGUID, Actor*> pair : m_Actors)
	{
		Actor* actor = pair.second;
		if (actor->IsTickEnabled())
		{
			m_TickList.push_back(actor);
		}
	}
}

void Level::DispatchBeginPlay()
{
	for (std::pair<FGUID, Actor*> pair : m_Actors)
	{
		pair.second->BeginPlay();
	}
}

void Level::DispatchEditorBeginPlay()
{
	for (std::pair<FGUID, Actor*> pair : m_Actors)
	{
		pair.second->EditorBeginPlay();
	}
}

void Level::Tick(float DeltaTime)
{
	for (auto& TickActor : m_TickList)
	{
		TickActor->Tick(DeltaTime);
	}
}

void Level::SetTickEnabled(Actor* TickActor, bool Enable)
{
	if (!TickActor)
	{
		FLOW_ENGINE_ERROR("Level::SetTickEnabled: Actor was nullptr");
		return;
	}

	if (TickActor->IsTickEnabled() == Enable)
	{
		FLOW_ENGINE_WARNING("Level::SetTickEnabled: Tick is already correctly set on actor %s", TickActor->GetName().c_str());
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