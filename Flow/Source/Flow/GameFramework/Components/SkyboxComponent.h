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
	
	//= Public Functions =================================

									SkyboxComponent();
									SkyboxComponent(const std::string& Name);

	virtual DirectX::XMMATRIX		GetTransformXM() const override;
	virtual void					Render() override;

	virtual std::string				GetClassSerializationUID(std::ofstream* Archive);

protected:

	//= Protected Variables ==============================

	MeshAsset*						m_Mesh;
	Material*						m_Material;									
	std::string						m_MeshPath = "Box";
	std::string						m_MaterialPath = "Mat_SkyCube";
};