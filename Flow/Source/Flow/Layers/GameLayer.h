#pragma once

//= Includes ===================================

#include "Layer.h"

//= Class Definition ===========================

class GameLayer : public Layer
{
public:

	//= Public Functions ================

					GameLayer();
	virtual			~GameLayer();

	virtual void	BeginPlay() override;
	virtual void	OnUpdate(float DeltaTime) override;
	virtual void	OnImGuiRender(bool DrawEditor) override;
	virtual void	OnAttach() override;
};