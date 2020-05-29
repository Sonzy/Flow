#pragma once
#include <DirectXMath.h>
#include "Flow/Helper/Maths.h"

/* Interface for camera, anything inheriting from this can be used as a camera.
Use Update instead of tick, if the camera is moved outside of update, re-cache */
class FLOW_API CameraBase
{
public:
	virtual void SetProjectionMatrix(DirectX::XMMATRIX NewProjection);

	[[nodiscard]] virtual DirectX::XMMATRIX GetViewMatrix() const = 0;


	//TODO: Not sure whether to keep this here
	virtual void Update(float DeltaTime);

	virtual void CacheMatrices();

	DirectX::XMMATRIX GetCachedViewProjection();
	DirectX::XMMATRIX GetTransposedCachedViewProjection();
	DirectX::XMMATRIX GetCachedView();

	DirectX::XMMATRIX GetProjectionMatrix() const;

	virtual Vector GetCameraPosition() const = 0;

	virtual void SetFOV(float NewFOV) { _FieldOfView = NewFOV; }
	virtual float GetFOV() const { return _FieldOfView; };

	void MarkCacheDirty() { _CacheDirty = true; }

protected:

	DirectX::XMMATRIX _CachedViewProj;
	DirectX::XMMATRIX _CachedView;
	DirectX::XMMATRIX _CachedTransposedViewProj;
	DirectX::XMMATRIX _Projection;

	bool _CacheDirty = false;
	Vector _LastKnownCameraPos;

	float _FieldOfView = 90;
};