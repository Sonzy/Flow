#pragma once
#include <unordered_map>

#include "UIComponent.h"

//= Forward Declarations ====================================

class World;
class Actor;

//= Class Definitions =======================================

class SpawnWindow : public UIComponent
{
public:

	//= Public Functions ====================================

						SpawnWindow();

	virtual void		Update() override;
	virtual void		Render() override;

	// Renders a popup context menu with a dropdown for spawning a component
	static void			DrawSpawnContextWindow(Actor* Parent);

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