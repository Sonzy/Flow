#pragma once

//= Includes ========================================

#include "Flow\GameFramework\Actor.h"

//= Forward Declarations ============================

class Pawn;
class CameraBase;
class WorldComponent;

//= Class Definitions ===============================

/* Base class used for allowing anything to control an object */
class FLOW_API Controller : public Actor
{
public:

	//= Public Functons ========================

								Controller();
								Controller(const std::string& Name);

	virtual void				ControlObject(Pawn* Obj);
	virtual void				RemoveControl(Pawn* Obj);
	void						SetCamera(CameraBase* NewCamera);
	CameraBase*					GetCamera() const;
	bool						IsLocalController() const;
	unsigned int				GetControllerIndex() const;

protected:

	//= Protected Variables ====================

	Pawn*						m_ControlledObject;
	CameraBase*					m_Camera;
								
	bool						m_LocalController = true;
	unsigned int				m_ControllerIndex = 0;

	WorldComponent*				m_Root;
};