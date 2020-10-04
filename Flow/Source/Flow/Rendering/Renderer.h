#pragma once

//= Includes ===================================

#include "Flow/Core.h"
#include "RenderCommand.h"
#include "Flow/Rendering/Core/Renderable.h"

//= Forward Declarations =======================

class RenderableComponent;
class Technique;

//= Class Definitons ===========================

class FLOW_API Renderer
{
public:
	
	//= Public Static Functions ========================================

	static void					BeginScene();
	static void					EndScene();
	static int					SubmitWithoutDraw(Renderable* Renderables);
	static void					Submit(Renderable* const Object);
	static void					Draw(int Count);

	//= Public Functions ===============================================

	virtual						~Renderer();
	virtual void				InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight) {}

private:

	//= Private Variables ==============================================

	static int					s_ObjectsRendered;
};