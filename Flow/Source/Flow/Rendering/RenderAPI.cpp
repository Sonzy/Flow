#include "Flowpch.h"
#include "RenderAPI.h"

RenderAPI::API RenderAPI::s_API = RenderAPI::API::DirectX11;

void RenderAPI::InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight)
{
	FLOW_ENGINE_ERROR("InitialiseDX11API: Not using DX11 API.");
}