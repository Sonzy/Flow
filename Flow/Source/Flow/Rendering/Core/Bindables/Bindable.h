#pragma once

// Includes ///////////////////////////////////////////////////////////////////////////

#include "Framework/Types/HashString.h"
#include <cassert>

// Namespace //////////////////////////////////////////////////////////////////////////

namespace Bindables
{
	// Class Definition ///////////////////////////////////////////////////////////////

	class Bindable
	{
	public:

		// Public Functions ///////////////////////////////////////////////////////////


		//= Bindable Interface =

		virtual void					Bind() = 0;
		//static HashString				GenerateID(/* Insert Construction Arguments */);
		virtual HashString				GetID();
		
	protected:

		// Private Variables ///////////////////////////////////////////////////////////

		HashString m_id = (size_t)0;
	};

	inline HashString Bindable::GetID()
	{
		assert(false && "Class hasn't been overwritten.");
		return m_id;
	}
}