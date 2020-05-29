#pragma once
#include "Flow\Assets\AssetBase.h"
#include <memory>
#include "Flow\Rendering\Core\Materials\Material.h"

class FLOW_API MaterialAsset : public AssetBase
{
public:

	MaterialAsset();
	~MaterialAsset();

	template<typename T>
	T* CreateMaterial()
	{
		T* NewMaterial = new T();
		_Material = NewMaterial;
		return NewMaterial;
	}

	Material* GetMaterial();

protected:

	void GenerateAssetSize();

	Material* _Material;
};