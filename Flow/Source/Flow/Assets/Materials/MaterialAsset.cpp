#include "Flowpch.h"
#include "MaterialAsset.h"

MaterialAsset::MaterialAsset()
	: m_Material(nullptr)
{

}

MaterialAsset::~MaterialAsset()
{
	delete m_Material;
}

void MaterialAsset::GenerateAssetSize()
{
	m_AssetSize = sizeof(*m_Material);
}


