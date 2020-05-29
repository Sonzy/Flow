#pragma once
#include "Flow/Core.h"
#include <d3d11.h>
#include <wrl.h>
#include "Flow/ErrorHandling/ErrorMacros.h"
#include "Flow/Rendering/RenderCommand.h"

class FLOW_API Bindable
{
public:
	virtual void Bind() = 0;

	//static std::shared_ptr<Bindable> Resolve();
	/* Generates the Unique ID for this bindable without an instance of the bindable */
	//static std::string GenerateUID();
	/* Returns the Unique ID of an existing bindable instance*/
	virtual std::string GetUID() const {
		assert(false && "Must be overriden if used");
		return "";
	};

public:
	bool _Bound = false;
	bool _CheckBound = false;
};