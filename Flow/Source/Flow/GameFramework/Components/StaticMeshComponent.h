#pragma once
#include "Flow\GameFramework\Components\RenderableComponent.h"

class btCollisionShape;
class btRigidBody;

namespace Flow
{
	class MeshAsset;
	class Material;
	class Mesh;

	class FLOW_API StaticMeshComponent : public RenderableComponent
	{
	public:
		StaticMeshComponent();
		StaticMeshComponent(const std::string& Name, MeshAsset* Mesh = nullptr, Material* Material = nullptr, int MeshIndex = 0);
		virtual ~StaticMeshComponent();
		

		void InitialiseComponent(MeshAsset* Mesh , Material* Material );
		virtual void Tick(float DeltaTime) override;

		void SetMeshAndMaterial(MeshAsset* Mesh, Material* Material, int MeshIndex = 0);
		void SetStaticMesh(const std::string& MeshName);
		void SetMaterial(Material* NewMaterial);

		virtual void Render() override;

		/* Renderable Component Interface */

		virtual void RefreshBinds() override;
		virtual DirectX::XMMATRIX GetTransformXM() override;


		//Temp - TODO: Actually use component movement for rendering
		Mesh* GetMesh() { return StaticMesh_; }

		void EnableOutlineDrawing(bool Enabled) { DrawOutline_ = Enabled; RefreshBinds(); }
		void DrawOutline();

		virtual void DrawComponentDetailsWindow() override;

		//= Physics ==========

		/* Generates new collision from the mesh */
		void GenerateCollision();
		void CreateRigidBody();

		virtual void InitialisePhysics() override;
		virtual btRigidBody* GetRigidBody() override;

		void SetSimulatePhysics(bool Simulate);


		//TODO: Temp movement with rigidbodies
		void MovePhysicsBody(Transform NewTransform);




	protected:

		bool SimulatePhysics_ = false;
		bool DrawOutline_ = false;

		Mesh* StaticMesh_;
		Material* Material_;

		btCollisionShape* Collision_;
		btRigidBody* Body_;

		std::vector<std::shared_ptr<Bindable>> OutlineEffect;
		bool CurrentlyOutlining = false; //Used to specify we are currently drawing an outline so get different values
		float OutlineThickness_ = 0.05f;
	};
}