#pragma once

#include "Flow/Core.h"
#include "Layer.h"

class FLOW_API LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);
	void PopLayer(Layer* layer);
	void PopOverlay(Layer* overlay);

	std::vector<Layer*>::iterator begin() { return _Layers.begin(); };
	std::vector<Layer*>::iterator end() { return _Layers.end(); };
private:
	std::vector<Layer*> _Layers;
	unsigned int _LayerInsertIndex = 0;
};