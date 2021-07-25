// Pch //////////////////////////////////////////////////////////////////////

#include "pch.h"

// Main Include /////////////////////////////////////////////////////////////

#include "CameraComponent.h"

// Includes /////////////////////////////////////////////////////////////////

#include <yaml-cpp/yaml.h>
#include "Framework/Input/Input.h"
#include "Framework/Utils/DebugDraw.h"

#if WITH_EDITOR
	#include "Assets/AssetSystem.h"
	#include "Editor/Editor.h"
	#include "Editor/IconManager.h"
#endif

// Function Definitions /////////////////////////////////////////////////////

CameraComponent::CameraComponent()
	: CameraComponent("Camera Component")
{
}

CameraComponent::CameraComponent(const std::string& Name)
	: WorldComponent(Name)
{
	m_Projection = DirectX::XMMATRIX();
	m_FieldOfView = 90.0f;
}

void CameraComponent::OnRegistered()
{
#if WITH_EDITOR
	IconManager::IconData iconData;
	iconData.m_texture = AssetSystem::GetAsset<TextureAsset>("Icon_Camera");
	Editor::Get().GetUIComponent<IconManager>()->RegisterIcon(GetGuid(), iconData);
#endif // WITH_EDITOR
}

#if WITH_EDITOR
void CameraComponent::IconUpdate(IconManager& iconManager)
{
	Vector3 screen = Renderer::WorldToScreen(GetWorldPosition());
	Icon& icon = iconManager.GetIcon(GetGuid());
	icon.m_position.x = screen.x;
	icon.m_position.y = screen.y;
	icon.m_alignment = Icon::Alignment::Centre;
	icon.RefreshBinds(iconManager); //TODO: Dont do this every frame
}
#endif // WITH_EDITOR

void CameraComponent::Update(float DeltaTime)
{
	CameraBase::Update(DeltaTime);
}

void CameraComponent::MoveCamera(const Transform& NewTransform)
{
	m_RelativeTransform = NewTransform;
}

Transform CameraComponent::GetCameraTransform() const
{
	return m_RelativeTransform;
}

void CameraComponent::CustomRender()
{
	// Draw a camera frustum //

	DirectX::XMMATRIX matrix = GetTransformXM();

	const float nearZ = 0.5f;
	const float farZ = 500.0f;
	const float zRatio = farZ / nearZ;

	const float width = 1.0f;
	const float height = (float)Renderer::GetWindowSize().y / (float)Renderer::GetWindowSize().x;

	const float nearX = width / 2.0f;
	const float nearY = height / 2.0f;
	const float farX = nearX * zRatio;
	const float farY = nearY * zRatio;

	Vector3 aTransformed = Maths::Transform(matrix, Vector3(-nearX, nearY, nearZ));
	Vector3 bTransformed = Maths::Transform(matrix, Vector3(nearX, nearY, nearZ));
	Vector3 cTransformed = Maths::Transform(matrix, Vector3(nearX, -nearY, nearZ));
	Vector3 dTransformed = Maths::Transform(matrix, Vector3(-nearX, -nearY, nearZ));
	Vector3 eTransformed = Maths::Transform(matrix, Vector3(-farX, farY, farZ));
	Vector3 fTransformed = Maths::Transform(matrix, Vector3(farX, farY, farZ));
	Vector3 gTransformed = Maths::Transform(matrix, Vector3(farX, -farY, farZ));
	Vector3 hTransformed = Maths::Transform(matrix, Vector3(-farX, -farY, farZ));

	DebugDraw::DrawLine(aTransformed, bTransformed);
	DebugDraw::DrawLine(bTransformed, cTransformed);
	DebugDraw::DrawLine(cTransformed, dTransformed);
	DebugDraw::DrawLine(dTransformed, aTransformed);

	DebugDraw::DrawLine(eTransformed, fTransformed);
	DebugDraw::DrawLine(fTransformed, gTransformed);
	DebugDraw::DrawLine(gTransformed, hTransformed);
	DebugDraw::DrawLine(hTransformed, eTransformed);

	DebugDraw::DrawLine(aTransformed, eTransformed);
	DebugDraw::DrawLine(bTransformed, fTransformed);
	DebugDraw::DrawLine(cTransformed, gTransformed);
	DebugDraw::DrawLine(dTransformed, hTransformed);
}

DirectX::XMMATRIX CameraComponent::GetViewMatrix() const
{
	PROFILE_FUNCTION();

	const DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Rotator WorldRotation = Rotator::AsRadians(GetWorldRotation());
	const DirectX::XMFLOAT3 WorldPosition = GetWorldPosition();

	//Get Camera Look Vector3
	const auto lookVector = DirectX::XMVector3Transform(Forward,
		DirectX::XMMatrixRotationRollPitchYaw(WorldRotation.Pitch, WorldRotation.Yaw, WorldRotation.Roll)
	);

	//Get the location the camera is looking
	DirectX::XMFLOAT3 fLookVector;
	DirectX::XMFLOAT3 camTarget;

	DirectX::XMStoreFloat3(&fLookVector, lookVector);
	camTarget.x = WorldPosition.x + fLookVector.x;
	camTarget.y = WorldPosition.y + fLookVector.y;
	camTarget.z = WorldPosition.z + fLookVector.z;

	//Return transformed Matrix with camera looking at a target
	DirectX::XMVECTOR vCamTarget = DirectX::XMLoadFloat3(&camTarget);
	DirectX::XMVECTOR camPosition = DirectX::XMLoadFloat3(&WorldPosition);

	//Rotate the up vector
	DirectX::XMVECTOR Position = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR Rotation = DirectX::XMQuaternionRotationRollPitchYaw(WorldRotation.Pitch, WorldRotation.Yaw, WorldRotation.Roll);

	DirectX::XMVECTOR Rotated = DirectX::XMVector3Rotate(Position, Rotation);
	return DirectX::XMMatrixLookAtLH(camPosition, vCamTarget, Rotated);
}

Vector3 CameraComponent::GetCameraPosition() const
{
	return GetWorldPosition();
}