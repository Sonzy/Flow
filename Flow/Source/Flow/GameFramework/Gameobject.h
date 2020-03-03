#pragma once
#include "Flow\Core.h"

namespace Flow
{
	class FLOW_API GameObject
	{
	public:
		GameObject(); 
		GameObject(const std::string& Name);

		virtual void Tick(float DeltaTime);

		const std::string& GetName();

	protected:

		std::string m_ObjectName;
	};
}