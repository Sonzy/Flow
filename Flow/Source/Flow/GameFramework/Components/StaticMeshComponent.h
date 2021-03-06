#pragma once

//= Includes ============================================================

#include "GameFramework\Components\RenderableComponent.h"
#include "Rendering\Core\Bindables\Stencil.h"

//= Forward Declarations ================================================

class btCollisionShape;
class btRigidBody;
class MeshAsset;
class MaterialAsset;
class Material;
class Mesh;
namespace YAML { class Emitter; }

//= Class Definitions ==================================================

class FLOW_API StaticMeshComponent : public RenderableComponent
{
public:

	REGISTER_CLASS(StaticMeshComponent)
	
	//= Public Functions ================================

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

	const std::string&			GetMeshIdentifier() const;
	const std::string&			GetMaterialIdentifier() const;

	//= Saving and loading ==========

	virtual void				Serialize(YAML::Emitter& Archive) override;
	virtual void				Deserialize(YAML::Node& Archive) override;

	virtual void				DefaultInitialise() override;

	//= Rendering ==========

	virtual void				Render() override;

	/* Renderable Component Interface */

	virtual void				RefreshBinds() override;

	//Temp - TODO: Actually use component movement for rendering
	Mesh*						GetMesh() { return m_StaticMesh; }

	void						EnableOutlineDrawing(bool Enabled) { m_DrawOutline = Enabled; RefreshBinds(); }

	virtual void				DrawComponentDetailsWindow() override;

	//= Physics ==========

	/* Generates new collision from the mesh */
	void						GenerateCollision();

	virtual void				InitialisePhysics() override;
	virtual void				DestroyPhysics() override;

	void						SetStencilMode(Bindables::Stencil::Mode NewMode);

public:

	//= Public Variables =================================

	bool						m_DrawWithoutDepth = false;	//TODO: Move

protected:

	//= Protected Variables ==============================

	bool						m_DrawOutline = false;
	float						m_OutlineThickness = 0.05f;

	//TODO: Rework how we store meshes
	Mesh*						m_StaticMesh;
	int							m_MeshIndex;
	std::string					m_MeshIdentifier;
	std::string					m_MaterialIdentifier;
	Material*					m_Material;

private:

	//= Private Functions ====================================

	void						DrawMeshSelector();
	void						DrawMaterialSelector();
};