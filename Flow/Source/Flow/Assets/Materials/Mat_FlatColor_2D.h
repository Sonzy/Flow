#pragma once
#include "Rendering\Core\Materials\Material.h"
#include "Maths/Vector3.h"

class FLOW_API Mat_FlatColor_2D : public Material
{
public:

	//= Public Functions =============================

	Mat_FlatColor_2D();

	/* Make sure to call refresh binds after, only use this with a new instance of flat
	colour otherwise it will change them all */
	void					SetColour(Vector3 NewColor);

	virtual void			BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout) override;

protected:

	//= Protected Functions =========================

	std::string				GenerateTag();

protected:

	//= Protected Variables =========================
	struct ColorBuffer
	{
		DirectX::XMFLOAT4 Color;
	} m_Color;
};