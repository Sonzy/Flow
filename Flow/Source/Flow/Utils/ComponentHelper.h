#pragma once
#include <string>

// Class Definition

class ComponentHelper
{
public:

	// Public Functions //////////////////////////////////////

	/* Creates a new component */
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
};