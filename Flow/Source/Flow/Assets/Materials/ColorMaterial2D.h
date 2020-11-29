#pragma once
#include "Rendering\Core\Materials\Material.h"
#include "Maths/Vector3.h"

//TODO: Rename
//Solid Color but without a normal
class FLOW_API ColorMaterial2D : public Material
{
public:

	//= Public Functions =============================

	ColorMaterial2D();

	/* Make sure to call refresh binds after, only use this with a new instance of flat
	colour otherwise it will change them all */
	void					SetColour(Vector3 NewColour);

	virtual void			BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout) override;

protected:

	//= Protected Functions =========================

	std::string				GenerateTag();

protected:

	//= Protected Variables =========================
	struct ColorBuffer
	{
		DirectX::XMFLOAT4 Colour;
	} m_Color;
};