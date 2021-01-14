#include "Flowpch.h"
#include "RenderAPI.h"

void RenderAPI::InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight)
{
	FLOW_ENGINE_ERROR("InitialiseDX11API: Not using DX11 API.");
}

void RenderAPI::SetNearPlane(float plane)
{
	m_NearPlane = plane;
}

void RenderAPI::SetFarPlane(float plane)
{
	m_FarPlane = plane;
}

float& RenderAPI::GetNearPlaneRef()
{
	return m_NearPlane;
}

float& RenderAPI::GetFarPlaneRef()
{
	return m_FarPlane;
}
