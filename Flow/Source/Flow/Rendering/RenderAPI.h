#pragma once
#include "Flow/Rendering/Core/Camera/Camera.h"

namespace Flow
{
	class FLOW_API RenderAPI
	{
	public:

		enum class API
		{
			None,
			DirectX11,
			DirectX12,
			Vulkan
		};

	public:
		virtual ~RenderAPI() {};

		virtual void InitialiseDX11API(HWND WindowHandle, int ViewportWidth, int ViewportHeight) {};

		virtual void SetClearColour(float R, float G, float B, float A) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(int Count) = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void Resize(int Width, int Height) = 0;

		virtual Vector GetScreenToWorldDirection(int X, int Y) = 0;

		virtual void EnableDepth() = 0;
		virtual void DisableDepth() = 0;

		static API GetAPI() { return s_API; };

	public:

		IntVector2D GetWindowSize() { return ViewportSize_; }
		
	private:
		static API s_API;

	protected:

		IntVector2D ViewportSize_;
		IntVector2D AdjustedViewportSize_;

		float NearPlane_;
		float FarPlane_;
	};
}