#pragma once
#include "Component.h"

namespace Flow
{
	class FLOW_API WorldComponent : public Component
	{
	public:
		WorldComponent();

		virtual void Tick(float DeltaTime) override;
	};
}