#include "Flowpch.h"
#include "LayerStack.h"

namespace Flow
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : Layers_)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		Layers_.emplace(Layers_.begin() + LayerInsertIndex_, layer);
		LayerInsertIndex_++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		Layers_.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto iterator = std::find(Layers_.begin(), Layers_.end(), layer);

		if (iterator != Layers_.end())
		{
			Layers_.erase(iterator);
			LayerInsertIndex_--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto iterator = std::find(Layers_.begin(), Layers_.end(), overlay);

		if (iterator != Layers_.end())
		{
			Layers_.erase(iterator);
		}
	}
}