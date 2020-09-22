#pragma once
#include "Core.h"
#include "Rendering\Core\Materials\Material.h"
#include "Rendering/Core/RenderQueue/Step.h"

/* Material with lighting and a single texture. */
class FLOW_API Mat_TexturedPhong : public Material
{
public:

	//= Public Functions ==============================

								Mat_TexturedPhong();

	virtual void				BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout) override;


public:

	//= Public Varaibles ===============================

	ObjectLightBuffer			m_LightCBuffer;
};