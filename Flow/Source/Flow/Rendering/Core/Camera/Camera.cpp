//= Includes =======================================================

#include "Flowpch.h"
#include "Camera.h"

//= Class (Camera Base) Definition =================================

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
