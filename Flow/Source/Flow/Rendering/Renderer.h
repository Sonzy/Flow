#pragma once

// Includes /////////////////////////////////////////////////////////////

#include "Core.h"
#include "Rendering/Core/Renderable.h"
#include "Maths/IntVector2.h"

// Forward Declarations /////////////////////////////////////////////////

class DX11RenderAPI;
class FrameBuffer;
class RenderableComponent;
class Technique;

// Type Definitions //////////////////////////////////////////////////////

struct ID3D11Device;
struct ID3D11DeviceContext;
class CameraBase;

// Class Definiton //////////////////////////////////////////////////////

class FLOW_API Renderer
{
public:
	
	// Public Static Functions //////////////////////////////////////////

	static void							BeginScene();
	static void							EndScene();
	static int							SubmitWithoutDraw(Renderable* Renderables);
	static void							Submit(Renderable* const Object);
	static void							Draw(int Count);

	//= Commands =

	static void							Initialise(HWND WindowHandle, int ViewportWidth, int ViewportHeight);
	static void							Shutdown();
	static ID3D11Device*				GetDevice();
	static ID3D11DeviceContext*			GetContext();

	//= Camera =

	static CameraBase*					GetMainCamera();
	static void							SetMainCamera(CameraBase* NewCamera);
	static void							SetOrthographic();
	static void							SetPerspective();
	static void							SetNearPlane(float dist);
	static void							SetFarPlane(float dist);
	static float&						GetNearPlaneRef();
	static float&						GetFarPlaneRef();

	//= Window =

	static void							Resize(int Width, int Height);
	static void							ClearWindow();
	static void							SetClearColour(float R, float G, float B, float A);
	static IntVector2					GetWindowSize();
	static void							SetMinimized(bool Minimized);
	static bool							IsMinimized();

	//= Drawing =

	static void							DrawIndexed(int Count);
	static void							Draw(unsigned int Count);

	//= Frame =

	static void							BeginFrame();
	static void							EndFrame();

#if WITH_EDITOR
	static FrameBuffer*					GetEditorFrameBuffer();
	static void							BindEditorBuffer();
	static void							BindEditorBufferWithoutClear();
#endif
	static void							BindGameBuffer();
	static void							BindGameBufferWithoutClear();
	static FrameBuffer*					GetGameFrameBuffer();

	static void							BindFrameBuffer(FrameBuffer* Buffer);
	static void							BindBackBuffer();
	static void							BindFrameBufferWithoutClear(FrameBuffer* Buffer);
	static void							BindBackBufferWithoutClear();

	//= Helper =

	static Vector3						GetScreenToWorldDirectionVector(int X, int Y, IntVector2 WindowSize, IntVector2 Origin = IntVector2(0, 0));
	static IntVector2					WorldToScreen(Vector3 position);

	static RenderPass					GetActivePass();

private:

	// Private Static Variables /////////////////////////////////////////////////

	static DX11RenderAPI*				sm_renderApi;
	static CameraBase*					sm_mainCamera;
	static int							s_objectsRendered;
};