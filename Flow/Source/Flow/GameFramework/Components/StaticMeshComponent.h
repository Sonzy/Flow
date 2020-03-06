#pragma once
#include "Flow\GameFramework\Components\WorldComponent.h"
#include "Flow\Rendering\Core\Mesh\StaticMesh.h"

namespace Flow
{
	//class MeshAsset;

	class FLOW_API StaticMeshComponent : public WorldComponent
	{
	public:
		StaticMeshComponent();
		StaticMeshComponent(const std::string& Name);
		virtual ~StaticMeshComponent();
		

		void InitialiseComponent(const std::string& MeshName, Material* OverrideMaterial);
		virtual void Tick(float DeltaTime) override;

		void SetStaticMesh(const std::string& MeshName);
		void SetMaterial(Material* NewMaterial);

		virtual void Render() override;

	protected:

		StaticMesh* m_StaticMesh;
		bool bOverrideMaterial = true;

		Material* m_Material;
	};
}