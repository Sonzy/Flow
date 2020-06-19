#pragma once
#include <string>

class Actor;

class Level
{
public:
	Level() = delete;
	Level(const std::string& LevelName);

	void Save(std::ofstream& Output);
	void Load(std::ifstream& Input);

	void InitialiseTickList();
	void DispatchBeginPlay();
#if WITH_EDITOR
	void DispatchEditorBeginPlay();
#endif
	void Tick(float DeltaTime);
	void SetTickEnabled(std::shared_ptr<Actor> Actor, bool Enable);

	std::vector<std::shared_ptr<Actor>>& GetActors() { return _Actors; }

private:
	std::string _Name;
	std::vector<std::shared_ptr<Actor>> _Actors;
	std::vector<std::shared_ptr<Actor>> _TickList;
};