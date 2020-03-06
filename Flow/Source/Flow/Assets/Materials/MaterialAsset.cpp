#include "Flowpch.h"
#include "MaterialAsset.h"

namespace Flow
{
	MaterialAsset::MaterialAsset()
		: m_Material(nullptr)
	{

	}

	MaterialAsset::~MaterialAsset()
	{
		delete m_Material;
	}

	Material* MaterialAsset::GetMaterial()
	{
		return m_Material;
	}

	void MaterialAsset::GenerateAssetSize()
	{
		m_AssetSize = sizeof(*m_Material);
	}
}


