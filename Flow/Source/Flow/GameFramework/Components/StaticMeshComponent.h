#pragma once
#include "Flow\GameFramework\Components\RenderableComponent.h"

namespace Flow
{
	class MeshAsset;
	class Material;

	class FLOW_API StaticMeshComponent : public RenderableComponent
	{
	public:
		StaticMeshComponent();
		StaticMeshComponent(const std::string& Name, MeshAsset* Mesh = nullptr, Material* Material = nullptr);
		virtual ~StaticMeshComponent();
		

		void InitialiseComponent(MeshAsset* Mesh , Material* Material );
		virtual void Tick(float DeltaTime) override;

		void SetMeshAndMaterial(MeshAsset* Mesh, Material* Material);
		void SetStaticMesh(const std::string& MeshName);
		void SetMaterial(Material* NewMaterial);

		virtual void Render() override;

		/* Renderable Component Interface */

		virtual void RefreshBinds() override;


		//Temp - TODO: Actually use component movement for rendering
		MeshAsset* GetMesh() { return m_StaticMesh; }

	protected:

		MeshAsset* m_StaticMesh;
		bool bOverrideMaterial = true;
		Material* m_Material;
	};
}