#pragma once

//= Includes ===========================================

#include "Controller.h"

//= Forward Declarations ===============================

class CameraBase;
class WorldObject;

//= Class Declarations =================================

class PlayerController : public Controller
{
public:

	//= Public Function Definitions =============

									PlayerController();
									PlayerController(const std::string& Name);
	void							SetCamera(CameraBase* NewCamera);

protected:

	//= Protected Variables =====================

	CameraBase*						m_Camera;
	WorldObject*					m_Pawn;
};
