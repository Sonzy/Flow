#include "Flowpch.h"
#include "LayerStack.h"

LayerStack::LayerStack()
{
}

LayerStack::~LayerStack()
{
	for (Layer* layer : _Layers)
		delete layer;
}

void LayerStack::PushLayer(Layer* layer)
{
	_Layers.emplace(_Layers.begin() + _LayerInsertIndex, layer);
	_LayerInsertIndex++;
}

void LayerStack::PushOverlay(Layer* overlay)
{
	_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer)
{
	auto iterator = std::find(_Layers.begin(), _Layers.end(), layer);

	if (iterator != _Layers.end())
	{
		_Layers.erase(iterator);
		_LayerInsertIndex--;
	}
}

void LayerStack::PopOverlay(Layer* overlay)
{
	auto iterator = std::find(_Layers.begin(), _Layers.end(), overlay);

	if (iterator != _Layers.end())
	{
		_Layers.erase(iterator);
	}
}