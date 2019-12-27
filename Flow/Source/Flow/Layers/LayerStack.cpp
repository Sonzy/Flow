#include "Flowpch.h"
#include "LayerStack.h"

namespace Flow
{
	LayerStack::LayerStack()
	{
		LayerInsert = Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		LayerInsert = Layers.emplace(LayerInsert, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto iterator = std::find(Layers.begin(), Layers.end(), layer);

		if (iterator != Layers.end())
		{
			Layers.erase(iterator);
			LayerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto iterator = std::find(Layers.begin(), Layers.end(), overlay);

		if (iterator != Layers.end())
		{
			Layers.erase(iterator);
		}
	}
}