#pragma once
#include <DirectXMath.h>
#include "Flow\Helper\Maths.h"

namespace Flow
{
	/* Interface for camera, anything inheriting from this can be used as a camera. 
	Use Update instead of tick, if the camera is moved outside of update, re-cache */
	class CameraBase
	{
	public:

		virtual void SetProjection(DirectX::XMMATRIX Projection);

		[[nodiscard]] virtual DirectX::XMMATRIX GetView() const = 0;
		DirectX::XMMATRIX GetProjection() const;


		virtual void Update(float DeltaTime) = 0;
		virtual void CacheCameraMatrices();

		DirectX::XMMATRIX GetCachedViewProjection();
		DirectX::XMMATRIX GetTransposedCachedViewProjection();
		DirectX::XMMATRIX GetCachedView();
		
		virtual Vector GetCameraPosition() const = 0;

		virtual void SetFOV(float NewFOV) { FieldOfView_ = NewFOV; }
		virtual float GetFOV() const { return FieldOfView_; };

		void MarkCacheDirty() { CacheDirty_ = false; }

	protected:

		DirectX::XMMATRIX CachedViewProj_;
		DirectX::XMMATRIX CachedView_;
		DirectX::XMMATRIX CachedTransposedViewProj_;
		DirectX::XMMATRIX Projection_;

		bool CacheDirty_ = false;
		Vector LastKnownCameraPos_;

		float FieldOfView_ = 90;
	};




















}