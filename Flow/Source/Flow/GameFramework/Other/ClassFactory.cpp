#include "Flowpch.h"
#include "Flow/Application.h"

ClassFactory& ClassFactory::Get()
{
	return Application::GetApplication().GetClassFactory();
}
