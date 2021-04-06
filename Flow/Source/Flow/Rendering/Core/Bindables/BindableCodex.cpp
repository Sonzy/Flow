//= Includes ===================================================

#include "pch.h"
#include "BindableCodex.h"

//= Static Variable Definitions ================================

BindableCodex* BindableCodex::s_Codex = new BindableCodex();

//= Class (BindableCodex) Functions ============================

BindableCodex& BindableCodex::Get()
{
	return *s_Codex;
}