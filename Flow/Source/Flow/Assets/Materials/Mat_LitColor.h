#pragma once

// Includes ////////////////////////////////////////////////////////

#include "Core.h"
#include "Rendering\Core\Materials\Material.h"
#include "Maths/Vector3.h"
#include "MaterialCommon.h"

// Type Definition /////////////////////////////////////////////////

class FLOW_API Mat_LitColor : public Material
{
public:

	// Public Functions ////////////////////////////////////////////

	Mat_LitColor();

	/* Make sure to call refresh binds after, only use this with a new instance of flat
	colour otherwise it will change them all */
	void					SetColor(Vector3 NewColour);

	virtual void			BindMaterial(Step* RenderingStep, const VertexLayout& VertexLayout) const override;

protected:

	// Protected Functions ////////////////////////////////////////////

	std::string				GenerateTag() const;

protected:

	// Protected Variables ////////////////////////////////////////////

	MaterialCommon::Buffer::ColorFloat4				m_color;
	MaterialCommon::Buffer::ObjectLightProperties	m_lightProperties;
};