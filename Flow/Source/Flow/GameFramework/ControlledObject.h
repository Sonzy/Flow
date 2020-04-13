#pragma once
#include "Flow\GameFramework\WorldObject.h"

namespace Flow
{
	class Controller;

	class FLOW_API ControlledObject : public WorldObject
	{
	public:
		ControlledObject();
		ControlledObject(const std::string& Name);

		void Control(Controller* NewController);
		void RemoveControl();

		Controller* GetController() const;

	private:

		Controller* Controller_;
	};
}