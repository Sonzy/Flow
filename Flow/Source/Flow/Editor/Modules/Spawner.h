#pragma once
#include <unordered_map>

#include "Module.h"

// Forward Declarations /////////////////////////////////////

class World;
class Actor;

// Class Definitions ////////////////////////////////////////

class Spawner : public Module
{
public:

	// Public Functions /////////////////////////////////////

						Spawner();
	virtual void		Update() override;

	// Renders a popup context menu with a dropdown for spawning a component
	void				DrawComponentSpawnContextWindow(Actor* Parent);
	void				DrawActorSpawnContextWindow();

	// Public Template Functions ////////////////////////////

	template<typename T>
	void RegisterActorClass(std::string_view DisplayName)
	{
		m_actorClassMap.emplace(typeid(T).name(), DisplayName);
	}

	template<typename T>
	void RegisterComponentClass(std::string_view DisplayName)
	{
		m_componentClassMap.emplace(typeid(T).name(), DisplayName);
	}

private:
	
	// Private Variables /////////////////////////////////////

	World&											m_WorldReference;
	float											m_SpawnDistance;

	/* Type name, display name*/
	std::unordered_map<std::string, std::string>	m_actorClassMap;
	std::unordered_map<std::string, std::string>	m_componentClassMap;
};