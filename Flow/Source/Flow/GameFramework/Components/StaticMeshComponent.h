#pragma once
#include "Flow\GameFramework\Components\RenderableComponent.h"
#include "Flow\Rendering\Core\Bindables\Stencil.h"

class btCollisionShape;
class btRigidBody;

class MeshAsset;
class MaterialAsset;
class Material;
class Mesh;


class FLOW_API StaticMeshComponent : public RenderableComponent
{
public:
								StaticMeshComponent();
								StaticMeshComponent(const std::string& Name, const std::string& MeshName = "", const std::string& MaterialName = "", int MeshIndex = 0);
	virtual						~StaticMeshComponent();


	void						InitialiseComponent(const std::string& MeshName, const std::string& MaterialName);
	virtual void				Tick(float DeltaTime) override;
#if WITH_EDITOR
	virtual void				EditorBeginPlay() override;
	virtual void				OnViewportSelected() override;
	virtual void				OnViewportDeselected() override;
#endif
	virtual void				BeginPlay() override;

	void						SetMeshAndMaterial(const std::string& MeshName, const std::string& MaterialName, int MeshIndex = 0);
	void						SetStaticMesh(const std::string& MeshName);
	void						SetMaterial(const std::string& MaterialName);

	void						SetMeshAndMaterial(MeshAsset* NewMesh, MaterialAsset* NewMaterial, int MeshIndex = 0);
	void						SetStaticMesh(MeshAsset* NewMesh);
	void						SetMaterial(MaterialAsset* NewMaterial);

	//= Saving and loading ==========

	virtual std::string			GetClassSerializationUID(std::ofstream* Archive) override;
	virtual void				Serialize(std::ofstream* Archive) override;
	virtual void				Deserialize(std::ifstream* Archive, Actor* NewParent) override;

	//= Rendering ==========

	virtual void				Render() override;

	/* Renderable Component Interface */

	virtual void				RefreshBinds() override;
	virtual						DirectX::XMMATRIX GetTransformXM() const override;


	//Temp - TODO: Actually use component movement for rendering
	Mesh*						GetMesh() { return _StaticMesh; }

	void						EnableOutlineDrawing(bool Enabled) { _DrawOutline = Enabled; RefreshBinds(); }

	virtual void				DrawComponentDetailsWindow() override;

	//= Physics ==========

	/* Generates new collision from the mesh */
	void						GenerateCollision();

	virtual void				InitialisePhysics() override;
	virtual void				DestroyPhysics() override;

	void						SetStencilMode(StencilMode NewMode);

	//TODO: Move
	bool						m_DrawWithoutDepth = false;
protected:

	bool						_DrawOutline = false;


	//TODO: Rework how we store meshes
	Mesh*						_StaticMesh;
	int							_MeshIndex;
	std::string					_MeshIdentifier;
	std::string					_MaterialIdentifier;
	Material*					_Material;

	float						_OutlineThickness = 0.05f;
};