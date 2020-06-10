#pragma once
#include "Flow/GameFramework/Components/RenderableComponent.h"

class Material;
class MeshAsset;

class SkyboxComponent : public RenderableComponent
{
public:
	SkyboxComponent() = delete;
	SkyboxComponent(const std::string& Name);

	virtual DirectX::XMMATRIX GetTransformXM() const override;
	virtual void Render() override;

protected:

	MeshAsset* _Mesh;
	Material* _Material;

	std::string _MeshPath = "Box";
	std::string _MaterialPath = "Mat_SkyCube";
};