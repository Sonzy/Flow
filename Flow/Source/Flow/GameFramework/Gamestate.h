#pragma once

//= Class Definition ===============================================

class Gamestate
{
public:

	//= Public Functions ===========================================

	virtual void				BeginPlay();
	virtual void				Tick(float DeltaTime);
};