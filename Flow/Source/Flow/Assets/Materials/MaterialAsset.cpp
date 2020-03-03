#include "Flowpch.h"
#include "MaterialAsset.h"

namespace Flow
{
	Material* MaterialAsset::GetMaterial()
	{
		return m_Material;
	}

	void MaterialAsset::GenerateAssetSize()
	{
		m_AssetSize = sizeof(*m_Material);
	}
}


