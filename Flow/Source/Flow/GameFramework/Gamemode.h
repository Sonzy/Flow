#pragma once

//= Class Definition ===============================================

class Gamemode
{
public:

	//= Public Functions ===========================================

	virtual void				BeginPlay();
	virtual void				Tick(float DeltaTime);
};