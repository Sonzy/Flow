#pragma once
#include "Layer.h"

class GameLayer : public Layer
{
public:

	GameLayer();
	virtual ~GameLayer();

	virtual void BeginPlay() override;
	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnImGuiRender(bool DrawEditor) override;
	virtual void OnAttach() override;
};