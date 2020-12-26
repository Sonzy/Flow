#pragma once

//= Includes ========================================================

#include "GameFramework/Components/RenderableComponent.h"

//= Forward Declarations ============================================

class Material;
class MeshAsset;

//= Class Definitions ===============================================

class SkyboxComponent : public RenderableComponent
{
public:

	REGISTER_CLASS(SkyboxComponent)
	
	//= Public Functions =================================

									SkyboxComponent();
									SkyboxComponent(const std::string& Name);

	virtual DirectX::XMMATRIX		GetTransformXM() const override;
	virtual void					Render() override;

protected:

	//= Protected Variables ==============================

	MeshAsset*						m_Mesh;
	Material*						m_Material;									
	std::string						m_MeshPath = "Box";
	std::string						m_MaterialPath = "Mat_SkyCube";
};