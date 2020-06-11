#pragma once
#include "Flow\Rendering\Core\Materials\Material.h"

#include "Flow\Rendering\Core\Bindables\ConstantBuffers\ShaderConstantBuffers.h"

class FLOW_API Mat_FlatColour : public Material
{
public:

	Mat_FlatColour();

	/* Make sure to call refresh binds after, only use this with a new instance of flat colour otherwise it will change them all */
	void SetColour(Vector NewColour);

	virtual void BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout) override;

protected:

	struct ColorBuffer
	{
		DirectX::XMFLOAT4 Colour;
	} _Colour;

	std::string GenerateTag();
};