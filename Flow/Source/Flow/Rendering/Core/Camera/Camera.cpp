#include "Flowpch.h"
#include "Camera.h"

void CameraBase::SetProjectionMatrix(DirectX::XMMATRIX Projection)
{
	_Projection = Projection;
}

#if WITH_EDITOR
void CameraBase::SetSceneProjection(DirectX::XMMATRIX NewProjection)
{
	_SceneProjection = NewProjection;
}

DirectX::XMMATRIX CameraBase::GetSceneProjectionMatrix() const
{
	return _SceneProjection;
}
#endif

DirectX::XMMATRIX CameraBase::GetProjectionMatrix() const
{
	return _Projection;
}

void CameraBase::CacheMatrices()
{
	_CachedView = GetViewMatrix();
	_CachedViewProj = _CachedView * GetProjectionMatrix();
	_CachedTransposedViewProj = DirectX::XMMatrixTranspose(_CachedViewProj);
	
	_CacheDirty = false;
}

void CameraBase::Update(float DeltaTime)
{
}

DirectX::XMMATRIX CameraBase::GetCachedViewProjection()
{
	if (_CacheDirty)
		return _CachedViewProj;

	CacheMatrices();
	return _CachedViewProj;
}

DirectX::XMMATRIX CameraBase::GetTransposedCachedViewProjection()
{
	if (_CacheDirty)
		return _CachedTransposedViewProj;

	CacheMatrices();
	return _CachedTransposedViewProj;
}

DirectX::XMMATRIX CameraBase::GetCachedView()
{
	if (_CacheDirty)
		return _CachedView;

	CacheMatrices();
	return _CachedView;
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
