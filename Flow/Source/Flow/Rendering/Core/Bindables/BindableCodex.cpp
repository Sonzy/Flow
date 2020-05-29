#include "Flowpch.h"
#include "BindableCodex.h"

BindableCodex* BindableCodex::s_Codex = new BindableCodex();

BindableCodex& BindableCodex::Get()
{
	return *s_Codex;
}