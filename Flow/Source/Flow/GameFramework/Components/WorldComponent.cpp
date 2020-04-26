#include "Flowpch.h"
#include "WorldComponent.h"
#include "Flow\GameFramework\WorldObject.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "Flow\GameFramework\World.h"

#include "ThirdParty\ImGui\imgui.h"

#include "Flow\Editor\Inspector.h"

namespace Flow
{
	WorldComponent::WorldComponent()
		: WorldComponent("WorldComponent")
	{
	}

	WorldComponent::WorldComponent(const std::string& Name)
		: Component(Name), ParentComponent_(nullptr), PhysicsMode_(PhysicsMode::Disabled),
		Collision_(nullptr), Rigidbody_(nullptr), MotionState_(nullptr), OverwriteWorldGravity_(true),
		ObjectGravity_(0.0f, -9.81f, 0.0f),	Mass_(1.0f), Visible_(true)
	{
	}

	WorldComponent::~WorldComponent()
	{
		Children_.clear();
		delete Collision_;
		delete Rigidbody_;
		delete MotionState_;
	}

	void WorldComponent::BeginPlay()
	{
		Component::BeginPlay();

		for (auto& Child : Children_)
		{
			Child->BeginPlay();
		}
	}

	void WorldComponent::Tick(float DeltaTime)
	{
		PROFILE_FUNCTION();

		//Update transform from physics world
		if (PhysicsMode_ == PhysicsMode::Dynamic)
		{
			PROFILE_CURRENT_SCOPE("Physics Update");

			btTransform InterpTransform;
			btScalar m[16];

			MotionState_->getWorldTransform(InterpTransform);		
			//InterpTransform = Rigidbody_->getWorldTransform();
			InterpTransform.getOpenGLMatrix(m);
			btVector3 Vec = InterpTransform.getOrigin();

			SetWorldPosition(Vector(Vec.x(), Vec.y(), Vec.z()));
			SetWorldRotation(Rotator::AsDegrees(Rotator(-asinf(m[9]), atan2f(m[1], m[5]), atan2f(m[8], m[10]))));
		}

		//Update the child components
		for (auto& Child : Children_)
		{
			Child->Tick(DeltaTime);
		}
	}

	void WorldComponent::AddChild(WorldComponent* Child)
	{
		CHECK_RETURN(!Child, "WorldComponent::AddChild: New Child was nullptr");
		Children_.push_back(Child);
		Child->SetParentComponent(this);
	}

	Vector WorldComponent::GetWorldPosition() const
	{
		PROFILE_FUNCTION();

		WorldComponent* Parent = GetParentComponent(); //TODO: Rotate this by the parents rotation
		//return Parent ? Parent->GetWorldPosition() + RelativeTransform_.Position_ : RelativeTransform_.Position_;

		if (Parent)
		{
			PROFILE_CURRENT_SCOPE("Parent Transform Calculation");
			Rotator ParentRotation = Rotator::AsRadians(Parent->GetWorldRotation());

			DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&RelativeTransform_.Position_.ToDXFloat3());
			DirectX::XMVECTOR Rotation = DirectX::XMQuaternionRotationRollPitchYaw(ParentRotation.Pitch, ParentRotation.Yaw, ParentRotation.Roll);
			DirectX::XMVECTOR NewPosition = DirectX::XMVector3Rotate(Position, Rotation);
			DirectX::XMFLOAT3 RotatedPos;
			DirectX::XMStoreFloat3(&RotatedPos, NewPosition);

			return Parent->GetWorldPosition() + Vector(RotatedPos.x, RotatedPos.y, RotatedPos.z);
		}
		else
			return RelativeTransform_.Position_;
	}

	Vector WorldComponent::GetRelativePosition() const
	{
		return RelativeTransform_.Position_;
	}

	void WorldComponent::SetWorldPosition(Vector NewPosition)
	{
		Vector CurrentParentWorld;
		
		while (WorldComponent* Parent = GetParentComponent())
		{
			CurrentParentWorld += Parent->RelativeTransform_.Position_;
		}

		RelativeTransform_.Position_ = NewPosition - CurrentParentWorld;
	}

	void WorldComponent::SetRelativePosition(Vector NewPosition)
	{
		RelativeTransform_.Position_ = NewPosition;
	}

	void WorldComponent::AddRelativePosition(Vector Position)
	{
		RelativeTransform_.Position_ += Position;
	}

	Rotator WorldComponent::GetWorldRotation() const
	{
		WorldComponent* Parent = GetParentComponent();
		return Parent ? Parent->GetWorldRotation() + RelativeTransform_.Rotation_ : RelativeTransform_.Rotation_;
	}

	Rotator WorldComponent::GetRelativeRotation() const
	{
		return RelativeTransform_.Rotation_;
	}

	void WorldComponent::SetWorldRotation(Rotator NewRotation)
	{
		Rotator CurrentParentWorld;

		if (WorldComponent* Parent = GetParentComponent())
			CurrentParentWorld = Parent->GetWorldRotation();

		RelativeTransform_.Rotation_ = NewRotation - CurrentParentWorld;

		//TODO: UpdatePhysics Movement
	}

	void WorldComponent::SetRelativeRotation(Rotator NewRotation)
	{
		RelativeTransform_.Rotation_ = NewRotation;
		//TODO: UpdatePhysics Movement
	}

	void WorldComponent::AddRelativeRotation(Rotator Rotation)
	{
		RelativeTransform_.Rotation_ += Rotation;
	}

	Vector WorldComponent::GetWorldScale() const
	{
		WorldComponent* Parent = GetParentComponent();
		return Parent ? Parent->GetWorldScale() * RelativeTransform_.Scale_ : RelativeTransform_.Scale_;
	}

	Vector WorldComponent::GetRelativeScale() const
	{
		return RelativeTransform_.Scale_;
	}

	void WorldComponent::SetWorldScale(Vector NewScale)
	{
		Vector CurrentParentWorld;

		while (WorldComponent* Parent = GetParentComponent())
		{
			CurrentParentWorld += Parent->RelativeTransform_.Scale_;
		}

		RelativeTransform_.Scale_ = NewScale - CurrentParentWorld;
	}

	void WorldComponent::SetRelativeScale(Vector NewScale)
	{
		RelativeTransform_.Scale_ = NewScale;
	}

	Transform WorldComponent::GetWorldTransform() const
	{
		return Transform(GetWorldPosition(), GetWorldRotation(), GetWorldScale());
	}

	Transform WorldComponent::GetRelativeTransform() const
	{
		return RelativeTransform_;
	}

	void WorldComponent::SetWorldTransform(Transform NewTransform)
	{
		SetWorldPosition(NewTransform.Position_);
		SetWorldRotation(NewTransform.Rotation_);
		SetWorldScale(NewTransform.Scale_);
	}

	void WorldComponent::SetRelativeTransform(Transform NewTransform)
	{
		RelativeTransform_ = NewTransform;
	}

	bool WorldComponent::InitialisePhysics(PhysicsMode Mode)
	{
		CHECK_RETURN_FALSE(Mode == PhysicsMode::Disabled, "WorldComponent::InitialisePhysics: Tried to init physics with physics disabled.");
		PhysicsMode_ = Mode;
		
		bool Failed = false;
		Failed |= !CreateCollision();
		Failed |= !CreateRigidBody();
		
		return !Failed;
	}

	void WorldComponent::InitialiseSubComponentPhysics(PhysicsMode Mode)
	{
		for(auto& Child : Children_)
		{
			if (!Child->InitialisePhysics(Mode))
				continue;

			if(Child->GetRigidBody() && Child->GetRigidBody()->getCollisionShape())
				World::GetWorld()->AddPhysicsObject(Child->GetRigidBody());
		}
	}

	bool WorldComponent::CreateCollision()
	{
		FLOW_ENGINE_ERROR("WorldComponent::CreateCollision: Object {0} - No mesh to create a collision, make sure you use a component with collision to use physics.", GetName());
		return false;
	}

	bool WorldComponent::CreateRigidBody()
	{
		CHECK_RETURN_FALSE(!Collision_, "WorldComponent::CreateRigidBody: Object {0} - Tried to create rigidbody without a collision", GetName());
		//Convert transform into BT Quaternion
		btQuaternion Rotation;
		Rotator Rot = Rotator::AsRadians(GetWorldRotation());
		Vector Pos = GetWorldPosition();
		Rotation.setEulerZYX(Rot.Roll, Rot.Yaw, Rot.Pitch);

		//Setup the motion state
		btVector3 Position = btVector3(Pos.X, Pos.Y, Pos.Z);
		MotionState_ = new btDefaultMotionState(btTransform(Rotation, Position));

		//Initialise the mass and intertia values
		btScalar bodyMass = (PhysicsMode_ == PhysicsMode::Static ||	PhysicsMode_ == PhysicsMode::Kinematic) ? 0 : Mass_;
		btVector3 bodyInertia;
		Collision_->calculateLocalInertia(bodyMass, bodyInertia);

		//Create the construction info for the body
		btRigidBody::btRigidBodyConstructionInfo bodyCI =
			btRigidBody::btRigidBodyConstructionInfo(bodyMass, MotionState_, Collision_, bodyInertia);
		bodyCI.m_restitution = 0.0f;
		bodyCI.m_friction = 0.5f;
		bodyCI.m_rollingFriction = 0.2f;
		bodyCI.m_spinningFriction = 0.3f;

		Rigidbody_ = new btRigidBody(bodyCI);
		Rigidbody_->setUserPointer(this);
		
		if (PhysicsMode_ == PhysicsMode::Kinematic)
		{
			Rigidbody_->setCollisionFlags(Rigidbody_->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		}

		//if(PhysicsMode_ != PhysicsMode::Dynamic)
		//	Rigidbody_->setActivationState(DISABLE_DEACTIVATION);
		Rigidbody_->setActivationState(DISABLE_DEACTIVATION);
			//Rigidbody_->setCollisionFlags(Rigidbody_->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

		if (UsingCustomGravity())
		{
			Rigidbody_->setFlags(Rigidbody_->getFlags() | BT_DISABLE_WORLD_GRAVITY);
			Rigidbody_->setGravity(btVector3(ObjectGravity_.X, ObjectGravity_.Y, ObjectGravity_.Z));
		}
		return true;
	}

	void WorldComponent::MovePhysicsBody(Transform NewTransform)
	{
		PROFILE_FUNCTION();

		btMotionState* motionState = Rigidbody_->getMotionState();
		btTransform Transform;
		btQuaternion Rotation;
		Rotator RadiansRotation = Rotator::AsRadians(NewTransform.Rotation_);
		Rotation.setEuler(RadiansRotation.Yaw, RadiansRotation.Pitch, RadiansRotation.Roll);

		//Set new transform
		Transform.setOrigin(btVector3(NewTransform.Position_.X, NewTransform.Position_.Y, NewTransform.Position_.Z));
		Transform.setRotation(Rotation);

		//Set Scale
		Collision_->setLocalScaling(btVector3(NewTransform.Scale_.X, NewTransform.Scale_.Y, NewTransform.Scale_.Z));

		//Update Transform
		Rigidbody_->setWorldTransform(Transform);
		motionState->setWorldTransform(Transform);
		World::GetPhysicsWorld()->updateSingleAabb(Rigidbody_);

		//Re-enable physics body
		Rigidbody_->activate();
	}

	void WorldComponent::UpdatePhysicsBody(bool ForceUpdate)
	{
		//TODO:
		//CHECK_RETURN(PhysicsMode_ == PhysicsMode::Disabled, "WorldComponent::UpdatePhysicsBody: Tried to move physics body when physics is disabled");

		if (PhysicsMode_ == PhysicsMode::Disabled)
		{
			return;
		}

		if((ForceUpdate || PhysicsMode_ == PhysicsMode::Dynamic) && GetRigidBody())
			MovePhysicsBody(GetWorldTransform());

		for (auto& Child : Children_)
		{
			if (Child->GetRigidBody() && Child->GetRigidBody()->getCollisionShape())
				Child->UpdatePhysicsBody(ForceUpdate);
		}
	}

	btRigidBody* WorldComponent::GetRigidBody() const
	{
		return Rigidbody_;
	}

	btCollisionShape* WorldComponent::GetCollision() const
	{
		return Collision_;
	}

	float WorldComponent::GetMass() const
	{
		return Mass_;
	}

	void WorldComponent::SetObjectGravity(Vector NewGravity)
	{
		ObjectGravity_ = NewGravity;
		OverwriteWorldGravity_ = true;
	}

	void WorldComponent::DisableGravity()
	{
		ObjectGravity_ = Vector(0.0f, 0.0f, 0.0f);
		OverwriteWorldGravity_ = true;
	}

	void WorldComponent::OverwriteWorldGravity(bool Overwrite)
	{
		OverwriteWorldGravity_ = Overwrite;
	}

	bool WorldComponent::UsingCustomGravity() const
	{
		return OverwriteWorldGravity_;
	}

	Vector WorldComponent::GetGravity() const
	{
		return ObjectGravity_;
	}

	void WorldComponent::Render()
	{
		PROFILE_FUNCTION();

		for (auto Child : Children_)
		{
			if(Child->IsVisible())
				Child->Render();
		}
	}


	Vector* WorldComponent::GetWriteablePosition()
	{
		return &RelativeTransform_.Position_;
	}
	Rotator* WorldComponent::GetWriteableRotation()
	{
		return &RelativeTransform_.Rotation_;
	}
	Vector* WorldComponent::GetWriteableScale()
	{
		return &RelativeTransform_.Scale_;
	}

	void WorldComponent::DrawInspectionTree(WorldComponent* CurrentInspectedComponent, bool DontOpenTree)
	{
		const ImVec2 ButtonSize = ImVec2(100, 12);
		const float RightOffset = 20;

		if (DontOpenTree)
			ImGui::SetNextItemOpen(false);

		bool NodeOpen = ImGui::TreeNode(GetName().c_str());
		
		if (ImGui::IsItemClicked())
			Inspector::UpdateSelectedComponent(this);

		if (!DontOpenTree && CurrentInspectedComponent == this)
		{
			ImGui::SameLine();
			ImGui::TextColored(IMGUI_GREEN, "Selected");
		}

		if (NodeOpen)
		{
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImU32 col = ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);

			//ImGui::RenderFrame(pos, ImVec2(pos.x + ImGui::GetContentRegionMax().x, pos.y + ImGui::GetTextLineHeight()), col, false);
			//
			//ImGuiContext& g = *GImGui;
			//ImGuiWindow* window = g.CurrentWindow;
			//window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
			//const float border_size = g.Style.FrameBorderSize;
			//if (border && border_size > 0.0f)
			//{
			//	window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), rounding, ImDrawCornerFlags_All, border_size);
			//	window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding, ImDrawCornerFlags_All, border_size);
			//}


			
			for (auto& Child : Children_)
			{
				Child->DrawInspectionTree(CurrentInspectedComponent, DontOpenTree);
			}
			ImGui::TreePop();
		}

	}

	void WorldComponent::DrawDetailsWindow(bool bDontUpdate)
	{
		Component::DrawDetailsWindow(bDontUpdate);

		ImGui::Checkbox("Component Visible", &Visible_);
	}

	std::vector<WorldComponent*> WorldComponent::GetChildren() const
	{
		return Children_;
	}
	void WorldComponent::SetVisibility(bool Visible)
	{
		Visible_ = Visible;
	}
	bool WorldComponent::IsVisible() const
	{
		return Visible_;
	}
}


