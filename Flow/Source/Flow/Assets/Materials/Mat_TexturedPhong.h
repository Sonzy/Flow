#pragma once
#include "Core.h"
#include "Rendering\Core\Materials\Material.h"
#include "MaterialCommon.h"

/* Material with lighting and a single texture. */
class FLOW_API Mat_TexturedPhong : public Material
{
public:

	//= Public Functions ==============================

								Mat_TexturedPhong();

	virtual void				BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout) const override;


public:

	//= Public Varaibles ===============================

	MaterialCommon::Buffer::ObjectLightProperties	m_lightProperties;
};