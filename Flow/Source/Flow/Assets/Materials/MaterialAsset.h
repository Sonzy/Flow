#pragma once
#include "Assets\Asset.h"
#include "Rendering\Core\Materials\Material.h"

class FLOW_API MaterialAsset : public Asset
{
public:

	//= Public Template Functions ============

	template<typename T>
	T* CreateMaterial()
	{
		T* NewMaterial = new T();
		m_Material = NewMaterial;
		return NewMaterial;
	}

	//= Public Fucntions =====================

	MaterialAsset();
	~MaterialAsset();

	Material* GetMaterial() { return m_Material; };

protected:
	//= Protected Functions ==================

	void GenerateAssetSize();

protected:

	//= Protected Variables ==================

	Material* m_Material;
};