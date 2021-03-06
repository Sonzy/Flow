#include "pch.h"
#include "CameraComponent.h"
#include "Framework\Input\Input.h"

#if WITH_EDITOR
#include "Assets/AssetSystem.h"
#include "Editor/Editor.h"
#include "Editor/IconManager.h"
#endif

#include <yaml-cpp/yaml.h>

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