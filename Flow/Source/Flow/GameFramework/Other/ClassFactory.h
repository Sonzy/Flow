#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <utility>
#include "Flow/Logging/Log.h"
#include "Flow/Helper/Profiling.h"

class ClassFactory
{
public:
	ClassFactory();
	void RegisterClassUIDs();

	static ClassFactory& Get();

	template<typename C>
	void RegisterFactoryClass()
	{
		//TODO: Better UID implementation
		//TODO: Limit class name character size
		std::string Hashed = typeid(C).name();

		std::function<void* (const ClassFactory&)> Temp = std::bind(&ClassFactory::CreateObject<C>, this);
		auto Value = std::pair<std::string, std::function<void* (const ClassFactory&)>>(Hashed, Temp);
		_FactoryMap.insert(Value);
	}

	template<typename T>
	T* CreateObjectFromID(const std::string& HashedName)
	{
		PROFILE_FUNCTION();

		auto Iterator = _FactoryMap.find(HashedName);

		if (Iterator == _FactoryMap.end())
		{
			FLOW_ENGINE_ERROR("ClassFactory::CreateObjectFromID: Failed to find class {0} in map", HashedName);
			return nullptr;
		}

		auto Func = *Iterator;
		return (T*)Func.second(*this);
	}

	template<typename T>
	void* CreateObject()
	{
		return (void*) new T();
	}

private:

	std::unordered_map <std::string, std::function<void*(const ClassFactory&)>> _FactoryMap;
};

