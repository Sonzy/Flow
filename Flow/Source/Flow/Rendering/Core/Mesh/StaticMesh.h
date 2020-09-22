#pragma once
#include "Flow/Core.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow/Rendering/Core/RenderableBase.h"
#include <DirectXMath.h>

class Material;

class FLOW_API StaticMesh : public RenderableBase
{
public:

	StaticMesh(const std::string& LocalPath);

	void InitialiseStaticMesh(const std::string& LocalPath, Material* MaterialOverride);

	virtual DirectX::XMMATRIX GetTransformXM() const override;

protected:

	Vector3 _Position;
	Rotator _Rotation;
	Vector3 _Scale;
};