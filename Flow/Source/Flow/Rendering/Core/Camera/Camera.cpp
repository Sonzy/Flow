//= Includes =======================================================

#include "pch.h"
#include "Camera.h"

#include "Assets/Materials/MaterialCommon.h"

//= Class (Camera Base) Definition =================================

CameraBase::CameraBase()
	//: m_cameraPixelBuffer(MaterialCommon::Register::Camera)
{
}

void CameraBase::SetProjectionMatrix(DirectX::XMMATRIX Projection)
{
	m_Projection = Projection;
}

#if WITH_EDITOR
void CameraBase::SetSceneProjection(DirectX::XMMATRIX NewProjection)
{
	m_SceneProjection = NewProjection;
}

DirectX::XMMATRIX CameraBase::GetSceneProjectionMatrix() const
{
	return m_SceneProjection;
}
#endif

DirectX::XMMATRIX CameraBase::GetProjectionMatrix() const
{
	return m_Projection;
}

void CameraBase::CacheMatrices()
{
	m_CachedView = GetViewMatrix();
	m_CachedViewProj = m_CachedView * GetProjectionMatrix();
	m_CachedTransposedViewProj = DirectX::XMMatrixTranspose(m_CachedViewProj);
	
	m_CacheDirty = false;
}

void CameraBase::Update(float DeltaTime)
{
	//Update from component position
	m_cameraBuffer.m_cameraPosition = GetCameraPosition();

	//Create a copy and transform the copied position with the view matrix
	CameraBuffer Copy = m_cameraBuffer;
	const DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&m_cameraBuffer.m_cameraPosition);

	//TODO: Might have to recalculate
	DirectX::XMStoreFloat3(&Copy.m_cameraPosition, DirectX::XMVector3Transform(Position, Renderer::GetMainCamera()->GetCachedView()));// modelView

	//Update the transformed position to the shader
	//m_cameraPixelBuffer.Update(Copy);
	//m_cameraPixelBuffer.Bind();
}

DirectX::XMMATRIX CameraBase::GetCachedViewProjection()
{
	if (m_CacheDirty)
		return m_CachedViewProj;

	CacheMatrices();
	return m_CachedViewProj;
}

DirectX::XMMATRIX CameraBase::GetTransposedCachedViewProjection()
{
	if (m_CacheDirty)
		return m_CachedTransposedViewProj;

	CacheMatrices();
	return m_CachedTransposedViewProj;
}

DirectX::XMMATRIX CameraBase::GetCachedView()
{
	if (m_CacheDirty)
		return m_CachedView;

	CacheMatrices();
	return m_CachedView;
}

bool CameraBase::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	return false;
}

bool CameraBase::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
{
	return false;
}

bool CameraBase::OnMouseMoved(MouseMovedEvent& e)
{
	return false;
}

bool CameraBase::OnMouseScrolled(MouseScrolledEvent& e)
{
	return false;
}

bool CameraBase::OnKeyPressed(KeyPressedEvent& e)
{
	return false;
}

bool CameraBase::OnKeyReleased(KeyReleasedEvent& e)
{
	return false;
}
