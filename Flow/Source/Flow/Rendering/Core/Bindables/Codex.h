#pragma once

// Includes ///////////////////////////////////////////////

#include "Rendering/Core/Bindables/Bindable.h"
#include <unordered_map>
#include "Framework/Types/HashString.h"

// Class Definiton ///////////////////////////////////////////////

namespace Bindables
{
	class Codex
	{
	public:

		template<class T, typename ...Params>
		static T* Resolve(Params&&...P)
		{
			//Generate the UID for this asset
			HashString GeneratedKey = T::GenerateID(std::forward<Params>(P)...);
			const auto FoundIndex = m_binds.find(GeneratedKey);

			//If we didnt find it, create a new one
			if (FoundIndex == m_binds.end())
			{
				T* Bind = new T(std::forward<Params>(P)...);
				m_binds[GeneratedKey] = Bind;
				return Bind;
			}
			else
				return static_cast<T*>(FoundIndex->second);
		}

		static std::unordered_map<HashString, Bindables::Bindable*> m_binds;
	};
}