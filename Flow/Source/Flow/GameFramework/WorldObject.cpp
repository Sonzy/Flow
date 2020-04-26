#include "Flowpch.h"
#include "WorldObject.h"
#include "Components\WorldComponent.h"
#include "ThirdParty\ImGui\imgui.h"

//TODO: Remove
#include "Components\StaticMeshComponent.h"

#include "ThirdParty\ImGui\misc\cpp\imgui_stdlib.h"
#include "Flow\GameFramework\World.h"

#include "Flow\Editor\Inspector.h"

namespace Flow
{
	WorldObject::WorldObject()
		: WorldObject("WorldObject")
	{

	}

	WorldObject::WorldObject(const std::string& Name)
		: GameObject(Name), RootComponent_(nullptr), PhysicsMode_(PhysicsMode::Disabled),
		Tag_(-1), Visible_(true), CurrentController_(nullptr)
	{
	}

	WorldObject::~WorldObject()
	{
		RootComponent_ = nullptr;
		FLOW_ENGINE_LOG("WorldObject::~WorldObject");
	}

	void WorldObject::BeginPlay()
	{
		PROFILE_FUNCTION();

		if (!RootComponent_)
			FLOW_ENGINE_ERROR("WorldObject::BeginPlay: Object {0} has no root component", ObjectName_);

		InitialisePhysics();

		RootComponent_->BeginPlay();

		BegunPlay_ = true;
	}

	void WorldObject::Tick(float DeltaTime)
	{
		PROFILE_FUNCTION();

		GameObject::Tick(DeltaTime);
		
		if(RootComponent_)
			RootComponent_->Tick(DeltaTime);
	}
	WorldComponent* WorldObject::GetRootComponent()
	{
		return RootComponent_;
	}

	Vector WorldObject::GetLocation()
	{
		if(!RootComponent_)
			return Vector();
		
		return RootComponent_->GetRelativePosition();
	}

	Vector WorldObject::GetScale()
	{
		if (!RootComponent_)
			return Vector();

		return RootComponent_->GetRelativeScale();
	}

	Rotator WorldObject::GetRotation()
	{
		if (!RootComponent_)
			return Rotator();

		return RootComponent_->GetRelativeRotation();
	}

	void WorldObject::Render()
	{
		if(RootComponent_ && Visible_ && RootComponent_->IsVisible())
			RootComponent_->Render();
	}

	PhysicsMode WorldObject::GetPhysicsMode() const
	{
		return PhysicsMode_;
	}

	void WorldObject::SetPhysicsMode(PhysicsMode NewMode)
	{
		PhysicsMode_ = NewMode;
		//TODO: Update all components
	}


	void WorldObject::InitialisePhysics()
	{
		if (PhysicsMode_ == PhysicsMode::Disabled)
			return;

		RootComponent_->InitialisePhysics(PhysicsMode_);

		//Allow static objects to have no collision on the root
		if (!RootComponent_->GetRigidBody())
		{
			if (PhysicsMode_ == PhysicsMode::Dynamic)
			{
				FLOW_ENGINE_ERROR("WorldObject::InitialisePhysics: Root Component had no rigidbody");
				return;
			}

			RootComponent_->InitialiseSubComponentPhysics(Flow::PhysicsMode::Static);
			return;
		}

		if (!RootComponent_->GetRigidBody()->getCollisionShape())
			return;

		World::GetWorld()->AddPhysicsObject(RootComponent_->GetRigidBody());

		RootComponent_->InitialiseSubComponentPhysics(Flow::PhysicsMode::Static);
	}

	btRigidBody* WorldObject::GetRigidBody()
	{
		return RootComponent_->GetRigidBody();
	}

	void WorldObject::DrawDetailsWindow(bool bDontUpdate)
	{
		PROFILE_FUNCTION();

		ImGui::TextColored(IMGUI_YELLOW, "Object Properties");

		ImGui::Separator();

		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::Checkbox("Is Visible", &Visible_);	
	}

	void WorldObject::OnComponentCollision(WorldComponent* Component, WorldComponent* Other)
	{
	}

	void WorldObject::SetVisibility(bool Visible)
	{
		Visible_ = Visible;
	}
}