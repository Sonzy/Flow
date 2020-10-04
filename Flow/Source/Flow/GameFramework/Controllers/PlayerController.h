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
	void							SetCamera(std::shared_ptr<CameraBase> NewCamera);

protected:

	//= Protected Variables =====================

	std::shared_ptr<CameraBase>		m_Camera;
	std::shared_ptr<WorldObject>	m_Pawn;
};
