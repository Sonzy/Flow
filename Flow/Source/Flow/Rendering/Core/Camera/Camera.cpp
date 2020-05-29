#include "Flowpch.h"
#include "Camera.h"

void CameraBase::SetProjectionMatrix(DirectX::XMMATRIX Projection)
{
	_Projection = Projection;
}

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

void CameraBase::SetProjectionMatrix(DirectX::XMMATRIX NewProjection)
{
	_Projection = NewProjection;
}

void CameraBase::Update(float DeltaTime)
{
}

void CameraBase::CacheMatrices()
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