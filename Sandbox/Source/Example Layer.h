#pragma once
#include "Flow/Layers/Layer.h"

class ExampleLayer : public Flow::Layer
{
public:
	ExampleLayer();

	virtual void OnImGuiRender() override;
	virtual void OnAttach() override;
};