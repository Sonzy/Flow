#pragma once

//= Includes =====================================

#include "Flow/Rendering/Core/Bindable.h"

//= Class Definition =============================

class Blender : public Bindable
{
public:

	//= Public Static Functions ===========================================

	static std::shared_ptr<Blender>		Resolve(bool Blending);
	static std::string					GenerateUID(bool Blending);

public:

	//= Public Functions ==================================================

										Blender(bool Blending);
	virtual void						Bind() override;
	std::string							GetUID() const override;

private:

	//= Private Variables =================================================

	Microsoft::WRL::ComPtr<ID3D11BlendState>		m_Blender;
	bool											m_Blending;
};