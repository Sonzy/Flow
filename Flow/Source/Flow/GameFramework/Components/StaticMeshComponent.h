#pragma once
#include "Flow\GameFramework\Components\RenderableComponent.h"
#include "Flow\Rendering\Core\Bindables\Stencil.h"

class btCollisionShape;
class btRigidBody;

class MeshAsset;
class Material;
class Mesh;

class FLOW_API StaticMeshComponent : public RenderableComponent
{
public:
	StaticMeshComponent();
	StaticMeshComponent(const std::string& Name, MeshAsset* Mesh = nullptr, Material* Material = nullptr, int MeshIndex = 0);
	virtual ~StaticMeshComponent();


	void InitialiseComponent(MeshAsset* Mesh, Material* Material);
	virtual void Tick(float DeltaTime) override;
#if WITH_EDITOR
	virtual void EditorBeginPlay() override;
#endif
	virtual void BeginPlay() override;

	void SetMeshAndMaterial(MeshAsset* Mesh, Material* Material, int MeshIndex = 0);
	void SetStaticMesh(const std::string& MeshName);
	void SetMaterial(Material* NewMaterial);

	virtual void Render() override;

	/* Renderable Component Interface */

	virtual void RefreshBinds() override;
	virtual DirectX::XMMATRIX GetTransformXM() const override;


	//Temp - TODO: Actually use component movement for rendering
	Mesh* GetMesh() { return _StaticMesh; }

	void EnableOutlineDrawing(bool Enabled) { _DrawOutline = Enabled; RefreshBinds(); }
	void DrawOutline();

	virtual void DrawComponentDetailsWindow() override;

	//= Physics ==========

	/* Generates new collision from the mesh */
	void GenerateCollision();

	virtual void InitialisePhysics() override;
	virtual void DestroyPhysics() override;
	//TODO: Temp movement with rigidbodies
	//void MovePhysicsBody(Transform NewTransform);

	void SetStencilMode(StencilMode NewMode);


protected:

	bool _SimulatePhysics = false;
	bool _DrawOutline = false;

	Mesh* _StaticMesh;
	Material* _Material;

	StencilMode _StencilMode;

	std::vector<std::shared_ptr<Bindable>> OutlineEffect;
	bool _CurrentlyOutlining = false; //Used to specify we are currently drawing an outline so get different values
	float _OutlineThickness = 0.05f;
};