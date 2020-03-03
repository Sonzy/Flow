#include "Flowpch.h"
#include "BindableCodex.h"

namespace Flow
{
	BindableCodex* BindableCodex::s_Codex = new BindableCodex();

	std::shared_ptr<Bindable> BindableCodex::GetBind(const std::string& Key)
	{
		auto i = s_Codex->m_Binds.find(Key);

		if (i == s_Codex->m_Binds.end())
			return nullptr;

		return i->second;
	}

	void BindableCodex::StoreBind(std::shared_ptr<Bindable> Bind)
	{
		s_Codex->m_Binds[Bind->GetUID()] = std::move(Bind);
	}
}