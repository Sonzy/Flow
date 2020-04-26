#include "Flowpch.h"
#include "BindableCodex.h"

namespace Flow
{
	BindableCodex* BindableCodex::s_Codex = new BindableCodex();


	BindableCodex& BindableCodex::Get()
	{
		return *s_Codex;
	}

}