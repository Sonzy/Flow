#pragma once
#include "Flow\Assets\AssetBase.h"
#include <memory>
#include "Flow\Rendering\Core\Materials\Material.h"

namespace Flow
{
	class FLOW_API MaterialAsset : public AssetBase
	{
	public:

		MaterialAsset()
		{};

		template<typename T>
		T* CreateMaterial()
		{
			T* NewMaterial = new T();
			m_Material = NewMaterial;
			return NewMaterial;
		}

		Material* GetMaterial();

	protected:

		void GenerateAssetSize();

		Material* m_Material;
	};
}