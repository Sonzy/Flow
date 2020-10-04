#pragma once

//= Includes ===================================

#include "Rendering\Core\Bindable.h"

//= Global Enums ===============================

enum class CullMode
{
	None = 1,
	Front = 2,
	Back = 3
};

//= Class Definition ===========================

class Rasterizer : public Bindable
{
public:

	//= Public Static Functions ===============================

	static std::shared_ptr<Rasterizer>				Resolve(CullMode CullMode);
	static std::string								GenerateUID(CullMode CullMode);

public:

	//= Public Functions ======================================

													Rasterizer(CullMode CullMode);
	void											Bind() override;

private:

	//= Private Variables ====================================

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_Rasterizer;
	CullMode										m_CullMode;
};