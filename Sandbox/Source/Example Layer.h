#pragma once
#include "Flow/Layers/Layer.h"

class ExampleLayer : public Flow::Layer
{
public:

	virtual void OnImGuiRender() override;
};