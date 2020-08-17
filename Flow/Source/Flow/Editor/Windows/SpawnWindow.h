#pragma once
#include <unordered_map>

class World;

class SpawnWindow
{
public:

	SpawnWindow(World* WorldRef);

	void Draw();

	template<typename T>
	void RegisterActorClass(std::string_view DisplayName)
	{
		_ActorClassMap.emplace(typeid(T).name(), DisplayName);
	}

private:

	World* _WorldReference;
	float _SpawnDistance;

	/* Type name, display name*/
	std::unordered_map<std::string, std::string> _ActorClassMap;
};