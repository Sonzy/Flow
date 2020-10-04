#pragma once

//= Includes ==================================

#include "Rendering/Core/Bindable.h"

//= Class Definition ==========================

class PixelShader : public Bindable
{
public:

	//= Public Static Functions ======================================

	static std::shared_ptr<Bindable>			Resolve(const std::string& LocalPath);
	static std::string							GenerateUID(const std::string& LocalPath);

public:

	//= Public Functions =============================================

												PixelShader(const std::string& LocalPath);
	void										Bind() override;
	std::string									GetUID() const override;

protected:

	//= Protected Variables ==========================================

	std::string									m_ShaderPath;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_PixelShader;
};