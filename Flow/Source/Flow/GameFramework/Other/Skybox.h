#pragma once
#include "Flow\Rendering\Core\Renderable.h"

class Material;
class MeshAsset;

class Skybox : public Renderable
{
public:
	Skybox();

	virtual DirectX::XMMATRIX GetTransformXM() const override;

private:

	MeshAsset* _Mesh;
	Material* _Material;

	std::string _MeshPath = "Box";
	std::string _MaterialPath = "Mat_SkyCube";
};