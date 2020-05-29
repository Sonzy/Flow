#include "Flowpch.h"
#include "MaterialAsset.h"

MaterialAsset::MaterialAsset()
	: _Material(nullptr)
{

}

MaterialAsset::~MaterialAsset()
{
	delete _Material;
}

Material* MaterialAsset::GetMaterial()
{
	return _Material;
}

void MaterialAsset::GenerateAssetSize()
{
	_AssetSize = sizeof(*_Material);
}


