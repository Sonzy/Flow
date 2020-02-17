#pragma once
#include "Flow\GameFramework\Components\WorldComponent.h"
#include "Flow\Rendering\Core\Mesh\StaticMesh.h"

namespace Flow
{
	class FLOW_API StaticMeshComponent : public WorldComponent
	{
	public:
		StaticMeshComponent();

		void InitialiseComponent(const std::string& MeshName);

		virtual void Tick(float DeltaTime) override;

	protected:

		std::shared_ptr<StaticMesh> m_StaticMesh;
	};
}