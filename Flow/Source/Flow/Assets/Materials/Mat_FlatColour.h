#pragma once
#include "Rendering\Core\Materials\Material.h"
#include "Maths/Vector3.h"
#include "MaterialCommon.h"

class FLOW_API Mat_FlatColour : public Material
{
public:
	
	//= Public Functions =============================

							Mat_FlatColour();

	/* Make sure to call refresh binds after, only use this with a new instance of flat
	colour otherwise it will change them all */
	void					SetColour(Vector3 NewColour);

	virtual void			BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout) const override;

protected:

	//= Protected Functions =========================

	std::string				GenerateTag() const;

protected:

	//= Protected Variables =========================

	MaterialCommon::Buffer::ColorFloat4				m_color;
};