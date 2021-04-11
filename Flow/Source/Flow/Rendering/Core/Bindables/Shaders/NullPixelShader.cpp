//= Includes ===============================================

#include "pch.h"
#include <d3d11.h>
#include "NullPixelShader.h"
#include "Flow/Rendering/Core/Bindables/Codex.h"
#include "Flow/Rendering/Renderer.h"

//= Class (Null Pixel Shader) Definition ===================
NullPixelShader::NullPixelShader()
{
}

void NullPixelShader::Bind()
{
	Renderer::GetContext()->PSSetShader(nullptr, nullptr, 0u);
}

NullPixelShader* NullPixelShader::Resolve()
{
	return Bindables::Codex::Resolve<NullPixelShader>();
}

HashString NullPixelShader::GenerateID()
{
	return "NullPixelShader";
}

HashString NullPixelShader::GetID()
{
	if (m_id.IsNull())
	{
		m_id = GenerateID();
	}
	return m_id;
}
