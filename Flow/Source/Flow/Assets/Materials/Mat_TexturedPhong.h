#pragma once
#include "Flow\Core.h"
#include "Flow\Rendering\Core\Materials\Material.h"
#include <DirectXMath.h>

/* Material with lighting and a single texture. */
class FLOW_API Mat_TexturedPhong : public Material
{
public:

	Mat_TexturedPhong();

	virtual void BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout) override;

	ObjectLightBuffer _LightCBuffer;
};