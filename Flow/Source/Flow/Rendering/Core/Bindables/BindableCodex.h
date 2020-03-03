#pragma once
#include "Flow\Rendering\Core\Bindable.h"
#include <memory>
#include <unordered_map>

namespace Flow
{
	class BindableCodex
	{
	public:
		static std::shared_ptr<Bindable> GetBind(const std::string& Key);
		static void StoreBind(std::shared_ptr<Bindable> Bind);

	private:
		std::unordered_map<std::string, std::shared_ptr<Bindable>> m_Binds;

	private:
		static BindableCodex* s_Codex;
	};
}