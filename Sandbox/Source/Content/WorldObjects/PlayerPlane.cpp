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

#include "ThirdParty\ImGui\imgui.h"

#include <functional>
#include "Flow\GameFramework\Components\WorldComponent.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

PlayerPlane::PlayerPlane()
	: PlayerPlane("Plane")
{

}

PlayerPlane::PlayerPlane(const std::string& Name)
	: Flow::ControlledObject(Name)
{
	PlaneMesh_ = CreateComponent<Flow::StaticMeshComponent>("Mesh Component");
	//PlayerCamera_ = CreateComponent<Flow::CameraComponent>("Camera Component");

	RootComponent_ = PlaneMesh_.get();
	//RootComponent_->AddChild(PlayerCamera_.get());

	auto Mesh = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("CharacterPlane");
	auto Material = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_CharacterPlane")->GetMaterial();
	PlaneMesh_->SetMeshAndMaterial(Mesh, Material);

	//PlayerCamera_->SetRelativePosition(Vector(0.0f, 8.0f, -20.0f));

	//Init Collision
	//SetPhysicsMode(Flow::PhysicsMode::Dynamic);
	SetPhysicsMode(Flow::PhysicsMode::Kinematic);
	RootComponent_->DisableGravity();


	//RootComponent_->OnObjectCollide.Bind(std::bind(&PlayerPlane::OnObjectHitPlane, *this, std::placeholders::_1, std::placeholders::_2));

	PlaneMesh_->UseCompoundCollision_ = true;
	PlaneMesh_->PlaneCollision = true;
	WASDMode = true;

	Alive = true;
}

void PlayerPlane::BeginPlay()
{
	WorldObject::BeginPlay();

	//Flow::Application::GetWorld()->GetLocalController()->ControlObject(this);
	//GetController()->SetCamera(PlayerCamera_.get());

	//Flow::Application::GetWorld()->GetLocalController()->SetCamera(PlayerCamera_.get());

}

void PlayerPlane::Tick(float DeltaTime)
{
	WorldObject::Tick(DeltaTime);

	//if (!WASDMode)
	//{
	//	if (Flow::Input::IsKeyPressed(FLOW_KEY_E))
	//		RootComponent_->SetRelativeRotation(RootComponent_->GetRelativeRotation() + Rotator(0.0f, -1.0f, 0.0f));
	//
	//	if (Flow::Input::IsKeyPressed(FLOW_KEY_Q))
	//		RootComponent_->SetRelativeRotation(RootComponent_->GetRelativeRotation() + Rotator(0.0f, 1.0f, 0.0f));
	//
	//	if (Flow::Input::IsKeyPressed(FLOW_KEY_SPACE))
	//		RootComponent_->SetRelativeRotation(RootComponent_->GetRelativeRotation().RotateRotator(Rotator(-1.0f, 0.0f, 0.0f)));
	//
	//	if (Flow::Input::IsKeyPressed(FLOW_KEY_SHIFT))
	//		RootComponent_->SetRelativeRotation(RootComponent_->GetRelativeRotation().RotateRotator(Rotator(1.0f, 0.0f, 0.0f)));
	//
	//	if (Alive)
	//	{
	//		//Move forward relative to the current rotation
	//		Vector Rotated = RootComponent_->GetWorldRotation().RotateVector(Vector(0.0f, 0.0f, PlaneSpeed_));
	//		RootComponent_->AddRelativePosition(Rotated);
	//	}
	//}
	//else
	//{
	//	IntVector2D Pos = Flow::Input::GetMousePosition();
	//	Rotator Rot = RootComponent_->GetWorldRotation();
	//
	//	if (Flow::Input::IsKeyPressed(FLOW_KEY_W))
	//		RootComponent_->AddRelativePosition(Rot.RotateVector(Vector(0.0f, 0.0f, PlaneSpeed_)));
	//	if (Flow::Input::IsKeyPressed(FLOW_KEY_A))
	//		RootComponent_->AddRelativePosition(Rot.RotateVector(Vector(-PlaneSpeed_, 0.0f, 0.0f)));
	//	if (Flow::Input::IsKeyPressed(FLOW_KEY_S))
	//		RootComponent_->AddRelativePosition(Rot.RotateVector(Vector(0.0f, 0.0f, -PlaneSpeed_)));
	//	if (Flow::Input::IsKeyPressed(FLOW_KEY_D))
	//		RootComponent_->AddRelativePosition(Rot.RotateVector(Vector(PlaneSpeed_, 0.0f, 0.0f)));
	//	if (Flow::Input::IsKeyPressed(FLOW_KEY_SHIFT))
	//		RootComponent_->AddRelativePosition(Rot.RotateVector(Vector(0.0f, -PlaneSpeed_, 0.0f)));
	//	if (Flow::Input::IsKeyPressed(FLOW_KEY_SPACE))
	//		RootComponent_->AddRelativePosition(Rot.RotateVector(Vector(0.0f, PlaneSpeed_, 0.0f)));
	//
	//
	//	if (Flow::Input::IsMousePressed(FLOW_MOUSE_RIGHT))
	//	{
	//		if (LastMousePosition_ != Pos)
	//		{
	//			Vector Direction = (LastMousePosition_ - Pos);
	//			Direction.X = Direction.X * 0.25f; //Horizontal Sensitivity
	//			Direction.Y = Direction.Y * 0.15f;  //Vertical Sensitivity
	//			RootComponent_->AddRelativeRotation(Rotator(-Direction.Y, 0.0f, -Direction.X));
	//		}
	//	}
	//	LastMousePosition_ = Pos;
	//}

	RootComponent_->UpdatePhysicsBody();

	//PlayerCamera_->MarkCacheDirty();
}

void PlayerPlane::DrawDetailsWindow(bool bDontUpdate)
{
	WorldObject::DrawDetailsWindow(bDontUpdate);

	ImGui::Checkbox("WASD Mode", &WASDMode);
	Alive ? ImGui::TextColored(IMGUI_GREEN, "Alive") : ImGui::TextColored(IMGUI_RED, "Dead");
	ImGui::InputFloat("Plane Speed", &PlaneSpeed_);
}

void PlayerPlane::OpenCVUpdate(float Angle, float Height)
{
	if (Alive)
	{
		float RollRotation = 0.0f;
		float PitchRotation = 0.0f;
		float AbsHeight = abs(Height);
		float UpdateAngle = (Angle + Math::PI);
		float MoveDiff = 0.0f;
		float HorMoveDiff = 0.0f;
		float RollAcceptance = 10.0f;

		RollRotation = UpdateAngle * (180.0f / Math::PI);
		PitchRotation = Math::Lerp(0.0f, PitchMax, AbsHeight / HeightMax);

		MoveDiff = Math::Lerp(0.0f, 1.0f, PitchRotation / PitchMax);
		HorMoveDiff = Math::Lerp(0.0f, 1.0f, (abs(RollRotation) > RollAcceptance) ? RollRotation / RollMax : 0.0f);

		RootComponent_->SetRelativeRotation(Rotator(Height >= 0 ? PitchRotation : -PitchRotation, RollRotation, 0.0f));
		//RootComponent_->AddRelativePosition(Vector(RollRotation >= 0 ? -HorMoveDiff : HorMoveDiff, (Height >= 0) ? -MoveDiff : MoveDiff, 0.0f));
		RootComponent_->AddRelativePosition(Vector(RollRotation >= 180 ? HorMoveDiff : -HorMoveDiff, (Height >= 0) ? -MoveDiff : MoveDiff, 0.0f));
		RootComponent_->UpdatePhysicsBody();

		FLOW_LOG("HorDiff: {0}, Roll: {1}", RollRotation >= 180 ? HorMoveDiff : -HorMoveDiff, RollRotation);
	}
}

void PlayerPlane::OnComponentCollision(Flow::WorldComponent* Component, Flow::WorldComponent* Other)
{
	KillPlane();
}


void PlayerPlane::KillPlane()
{
	Alive = false;

	GetRigidBody()->setFlags(GetRigidBody()->getFlags() & ~BT_DISABLE_WORLD_GRAVITY);
	GetRigidBody()->setCollisionFlags(GetRigidBody()->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);

	btVector3 bodyInertia;
	RootComponent_->GetCollision()->calculateLocalInertia(RootComponent_->GetMass(), bodyInertia);
	GetRigidBody()->setMassProps(RootComponent_->GetMass(), bodyInertia);
	GetRigidBody()->setGravity(Flow::World::GetPhysicsWorld()->getGravity());
}

void PlayerPlane::Test(bool a)
{
}

void PlayerPlane::ToggleWASDMode()
{
	WASDMode = true;
}
