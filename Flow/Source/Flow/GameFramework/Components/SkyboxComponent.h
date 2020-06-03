#pragma once
#include "Flow/GameFramework/Components/RenderableComponent.h"

class Material;
class MeshAsset;

class SkyboxComponent : public RenderableComponent
{
	SkyboxComponent() = delete;
	SkyboxComponent(const std::string& Name);

	virtual ~SkyboxComponent();
	virtual DirectX::XMMATRIX GetTransformXM() const override;

	MeshAsset* _Mesh;
	Material* _Material;

	std::string _MeshPath = "Box";
	std::string _MaterialPath = "Mat_SkyCube";
};