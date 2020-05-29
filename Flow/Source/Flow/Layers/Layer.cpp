#include "Flowpch.h"
#include "Layer.h"
#include "Flow\Helper\HelperMacros.h"

Layer::Layer(const std::string& Name)
	:_Name(Name)
{
}

Layer::~Layer()
{
}

void Layer::BeginPlay()
{
}

void Layer::OnAttach()
{
}

void Layer::OnDetach()
{
}

void Layer::OnUpdate(float DeltaTime)
{
}

void Layer::OnEvent(Event& e)
{
}

void Layer::OnImGuiRender(bool DrawEditor)
{
}

const std::string& Layer::GetName() const
{
	return _Name;
}
