#pragma once
#include <unordered_map>

//= Forward Declarations ====================================

class World;

//= Class Definitions =======================================

class SpawnWindow
{
public:

	//= Public Functions ====================================

													SpawnWindow(World* WorldRef);
	void											Draw();

	//= Public Template Functions ===========================

	template<typename T>
	void RegisterActorClass(std::string_view DisplayName)
	{
		m_ActorClassMap.emplace(typeid(T).name(), DisplayName);
	}

private:
	
	//= Private Variables ===================================

	World*											m_WorldReference;
	float											m_SpawnDistance;

	/* Type name, display name*/
	std::unordered_map<std::string, std::string>	m_ActorClassMap;
};