#pragma once
#include "Flow/Core.h"
#include <d3d11.h>
#include <wrl.h>
#include "Flow/ErrorHandling/ErrorMacros.h"

namespace Flow
{
	class Bindable
	{
	public:

		virtual void Bind() = 0;
	};
}
