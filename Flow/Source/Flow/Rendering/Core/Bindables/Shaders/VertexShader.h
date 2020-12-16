#pragma once

//= Includes =====================================

#include "Rendering/Core/Bindable.h"

//= Class Definition =============================

class VertexShader : public Bindable
{
public:

	//= PubliC Static Functions ====================================

	static VertexShader*					Resolve(const std::string& LocalPath);
	static std::string						GenerateUID(const std::string& LocalPath);

	//= Public Functions ===========================================

											VertexShader(const std::string& LocalPath);
	void									Bind() override;
	ID3DBlob*								GetByteCode() const;
	std::string								GetUID() const override;

protected:
	
	//= Protected Variables ========================================

	std::string									m_ShaderPath;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_VertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob>			m_Blob;
};