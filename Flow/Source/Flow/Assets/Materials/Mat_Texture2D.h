#pragma once
#include "Core.h"
#include "Rendering\Core\Materials\Material.h"
#include "Maths/Vector3.h"
#include "MaterialCommon.h"

//Texture but without a normal
class FLOW_API Mat_Texture2D : public Material
{
public:

	//= Public Functions =============================

	Mat_Texture2D();

	virtual void			BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout) const override;

protected:

	//= Protected Functions =========================

	std::string				GenerateTag() const;
};