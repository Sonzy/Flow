#pragma once
#include "Flow\Rendering\Core\Renderable.h"

namespace Flow
{
	class Material;
	class MeshAsset;

	class Skybox : public Renderable
	{
	public:
		Skybox();

		virtual DirectX::XMMATRIX GetTransformXM() const override;

	private:

		MeshAsset* Mesh_;
		Material* Material_;

		std::string MeshPath_ = "Box";
		std::string MaterialPath_ = "Mat_SkyCube";
	};
}