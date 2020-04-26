#include "Flowpch.h"
#include "Camera.h"

namespace Flow
{
	void CameraBase::SetProjection(DirectX::XMMATRIX Projection)
	{
		Projection_ = Projection;
	}

	DirectX::XMMATRIX CameraBase::GetProjection() const
	{
		return Projection_;
	}

	void CameraBase::CacheCameraMatrices()
	{
		CachedView_ = GetView();
		CachedViewProj_ = CachedView_ * GetProjection();
		CachedTransposedViewProj_ = DirectX::XMMatrixTranspose(CachedViewProj_);
	
		CacheDirty_ = true;
	}

	DirectX::XMMATRIX CameraBase::GetCachedViewProjection()
	{
		if (CacheDirty_)
			return CachedViewProj_;

		CacheCameraMatrices();
		return CachedViewProj_;
	}

	DirectX::XMMATRIX CameraBase::GetTransposedCachedViewProjection()
	{
		if (CacheDirty_)
			return CachedTransposedViewProj_;

		CacheCameraMatrices();
		return CachedTransposedViewProj_;
	}

	DirectX::XMMATRIX CameraBase::GetCachedView()
	{
		if (CacheDirty_)
			return CachedView_;
			
		CacheCameraMatrices();
		return CachedView_;		
	}
}