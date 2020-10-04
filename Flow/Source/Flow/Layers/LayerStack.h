#pragma once

//= Includes =============================

#include "Flow/Core.h"
#include "Layer.h"

//= Class Definition =====================

class FLOW_API LayerStack
{
public:

	//= Public Functions ========================================

									LayerStack();
									~LayerStack();

	void							PushLayer(Layer* layer);
	void							PushOverlay(Layer* overlay);
	void							PopLayer(Layer* layer);
	void							PopOverlay(Layer* overlay);

	std::vector<Layer*>::iterator	begin() { return m_Layers.begin(); };
	std::vector<Layer*>::iterator	end() { return m_Layers.end(); };
private:

	//= Private Variables =======================================

	std::vector<Layer*>				m_Layers;
	unsigned int					m_LayerInsertIndex = 0;
};