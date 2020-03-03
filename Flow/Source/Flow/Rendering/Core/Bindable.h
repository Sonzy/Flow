#pragma once
#include "Flow/Core.h"
#include <d3d11.h>
#include <wrl.h>
#include "Flow/ErrorHandling/ErrorMacros.h"
#include "Flow/Rendering/RenderCommand.h"

namespace Flow
{
	class FLOW_API Bindable
	{
	public:
		virtual void Bind() = 0;
		virtual std::string GetUID() const = 0;

	public:
		bool bBound = false;
		bool bCheckBound = false;
	};
}
