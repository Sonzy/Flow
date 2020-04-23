#include "Flowpch.h"
#include "PlayerPlane.h"

#include "Flow\GameFramework\Components\StaticMeshComponent.h"
#include "Flow\GameFramework\Components\CameraComponent.h"

#include "Flow\Input\Input.h"

#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Assets\Materials\MaterialAsset.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"
#include "Flow\GameFramework\Controllers\PlayerController.h"

PlayerPlane::PlayerPlane()
	: PlayerPlane("Plane")
{

}

PlayerPlane::PlayerPlane(const std::string& Name)
{
	PlaneMesh_ = CreateComponent<Flow::StaticMeshComponent>("Mesh Component");
	PlayerCamera_ = CreateComponent<Flow::CameraComponent>("Camera Component");

	RootComponent_ = PlaneMesh_.get();
	RootComponent_->AddChild(PlayerCamera_.get());

	auto Mesh = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("CharacterPlane");
	auto Material = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_CharacterPlane")->GetMaterial();
	PlaneMesh_->SetMeshAndMaterial(Mesh, Material);

	PlayerCamera_->SetRelativePosition(Vector(0.0f, 8.0f, -20.0f));

	//Init Collision
	PlaneMesh_->SetSimulatePhysics(true);
	PlaneMesh_->SetGravityEnabled(false);

	PlaneMesh_->InitialisePhysics();
	HasCollision_ = true;
}

void PlayerPlane::BeginPlay()
{
	WorldObject::BeginPlay();

	Flow::Application::GetWorld()->GetLocalController()->ControlObject(this);
	GetController()->SetCamera(PlayerCamera_.get());
}

void PlayerPlane::Tick(float DeltaTime)
{
	WorldObject::Tick(DeltaTime);

	if (Flow::Input::IsKeyPressed(FLOW_KEY_E))
		RootComponent_->SetRelativeRotation(RootComponent_->GetRelativeRotation() + Rotator(0.0f, -1.0f, 0.0f));

	if (Flow::Input::IsKeyPressed(FLOW_KEY_Q))
		RootComponent_->SetRelativeRotation(RootComponent_->GetRelativeRotation() + Rotator(0.0f, 1.0f, 0.0f));

	if (Flow::Input::IsKeyPressed(FLOW_KEY_SPACE))
		RootComponent_->SetRelativeRotation(RootComponent_->GetRelativeRotation().RotateRotator(Rotator(-1.0f, 0.0f, 0.0f)));

	if (Flow::Input::IsKeyPressed(FLOW_KEY_SHIFT))
		RootComponent_->SetRelativeRotation(RootComponent_->GetRelativeRotation().RotateRotator(Rotator(1.0f, 0.0f, 0.0f)));

	//Move forward relative to the current rotation
	Vector Rotated = RootComponent_->GetWorldRotation().RotateVector(Vector(0.0f, 0.0f, PlaneSpeed_));
	RootComponent_->AddRelativePosition(Rotated);

	static_cast<Flow::StaticMeshComponent*>(RootComponent_)->MovePhysicsBody(RootComponent_->GetWorldTransform());
}
