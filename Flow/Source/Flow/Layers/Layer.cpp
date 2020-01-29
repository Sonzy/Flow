#include "Flowpch.h"
#include "Layer.h"

Flow::Layer::Layer(const std::string& Name)
	:Name(Name)
{
}

Flow::Layer::~Layer()
{
}

void Flow::Layer::OnAttach()
{
}

void Flow::Layer::OnDetach()
{
}

void Flow::Layer::OnUpdate()
{
}

void Flow::Layer::OnEvent(Event& e)
{
}

void Flow::Layer::OnImGuiRender()
{
}

const std::string& Flow::Layer::GetName() const
{
	return Name;
}
