#pragma once
#include <vector>
#include <string>
#include "GameFramework/Components/WorldComponent.h"

// Class Definition ////////////////////////////////////////////

class ComponentHelper
{
public:

	// Static Public Template Functions //////////////////////////////////////

	/* Creates a new component. Make sure to register the component */
	template <typename T>
	static T* CreateFreeComponent(const std::string& NewName)
	{
		static_assert(std::is_base_of<Component, T>::value, "Tried to create a component templated with a non-component type");
		return new T(NewName);
	}

	template<typename Functor>
	static void ComponentRecursion(Functor functor, WorldComponent* root)
	{
		if (root == nullptr)
		{
			return;
		}

		functor(root);

		for (WorldComponent* child : root->GetChildren())
		{
			ComponentRecursion(functor, child);
		}
	}

	// Static Public Functions //////////////////////////////////////

	static void BuildComponentArray(WorldComponent* Root, std::vector<WorldComponent*>& outArray);
};