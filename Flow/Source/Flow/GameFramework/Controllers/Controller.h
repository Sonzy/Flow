#pragma once
#include "Flow\GameFramework\WorldObject.h"

namespace Flow
{
	class ControlledObject;
	class CameraComponent;

	/* Base class used for allowing anything to control an object */
	class FLOW_API Controller : public WorldObject
	{
	public:
		Controller();
		Controller(const std::string& Name);

		virtual void ControlObject(ControlledObject* Obj);
		virtual void RemoveControl(ControlledObject* Obj);

		void SetCamera(CameraComponent* NewCamera);
		CameraComponent* GetCamera() const;

		bool IsLocalController() const;
		unsigned int GetControllerIndex() const;

	protected:
		ControlledObject* ControlledObject_;
		CameraComponent* Camera_;

		bool LocalController_ = true;
		unsigned int ControllerIndex_ = 0;
	};
}