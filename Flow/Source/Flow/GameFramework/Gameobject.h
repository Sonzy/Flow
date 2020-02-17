#pragma once
#include "Flow\Core.h"

namespace Flow
{
	class FLOW_API GameObject
	{
	public:
		GameObject();

		virtual void Tick(float DeltaTime);
	};
}