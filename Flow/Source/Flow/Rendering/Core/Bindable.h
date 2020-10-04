#pragma once

//= Includes =======================================

#include "Flow/Core.h"
#include <d3d11.h>
#include <wrl.h>
#include "Flow/ErrorHandling/ErrorMacros.h"
#include "Flow/Rendering/RenderCommand.h"

//= Forward Declarations ===========================

class Renderable;
class TechniqueProbe;

//= Class Definition ===============================

class FLOW_API Bindable
{
public:

	//= Public Functions ===========================================

	virtual void			Bind() = 0;
	virtual void			InitialiseParentReferences(const Renderable&) {};
	virtual void			AcceptProbe(TechniqueProbe&) {};

	//= Template interface for bindable codex. =======

	//static std::shared_ptr<Bindable> Resolve();
	/* Generates the Unique ID for this bindable without an instance of the bindable */
	//static std::string GenerateUID();
	/* Returns the Unique ID of an existing bindable instance*/
	virtual std::string GetUID() const {
		assert(false && "Must be overriden if used");
		return "";
	};
	//==========================================

public:

	//= Public Variables ===========================================

	bool					m_Bound			= false;
	bool					m_CheckBound	= false;
};