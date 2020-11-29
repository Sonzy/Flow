#pragma once

//= Includes ==========================================

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <utility>
#include "Flow/Logging/Log.h"
#include "Flow/Utils/Profiling.h"

//= Class Definition ==================================

class ClassFactory
{

	typedef std::function<void* (const ClassFactory&)> SpawnFunction;
public:

	//= Public Template Functions ========

	template<typename C>
	void RegisterFactoryClass()
	{
		//TODO: Better UID implementation
		//TODO: Limit class name character size
		std::string Hashed = typeid(C).name();

		std::function<void* (const ClassFactory&)> Temp = std::bind(&ClassFactory::CreateObject<C>, this);
		auto Value = std::pair<std::string, std::function<void* (const ClassFactory&)>>(Hashed, Temp);
		m_FactoryMap.insert(Value);
	}

	template<typename T>
	T* CreateObjectFromID(const std::string& HashedName)
	{
		PROFILE_FUNCTION();

		auto Iterator = m_FactoryMap.find(HashedName);

		if (Iterator == m_FactoryMap.end())
		{
			FLOW_ENGINE_ERROR("ClassFactory::CreateObjectFromID: Failed to find class %s in map", HashedName.c_str());
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


	//= Public Static Functions ===========

	static ClassFactory&	Get();

	//= Public Functions ==================

							ClassFactory();
	void					RegisterClassUIDs();

private:

	std::unordered_map <std::string, SpawnFunction> m_FactoryMap;
};

