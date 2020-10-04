#pragma once

//= Includes ===========================================

#include "Flow\Rendering\Core\Renderable.h"

//= Forward Declarations ===============================

class Material;
class MeshAsset;

//= Class Definition ===================================

class Skybox : public Renderable
{
public:

	//= Public Functions ===================

									Skybox();
	virtual DirectX::XMMATRIX		GetTransformXM() const override;

private:

	//= Private Variables ==================

	MeshAsset*						m_Mesh;
	Material*						m_Material;									
	std::string						m_MeshPath = "Box";
	std::string						m_MaterialPath = "Mat_SkyCube";
};