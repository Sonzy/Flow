#pragma once

#include <opencv2\core.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\highgui.hpp>

namespace Flow
{
	class TextureAsset;
	class Sprite;

	class FLOW_API OpenCVTesting
	{
	public:
		OpenCVTesting();
		void Update();

		void RenderToIMGUI();

		cv::Mat Frame_;
		cv::VideoCapture Capture_;
		
		TextureAsset* CaptureTexture_;

		Sprite* TestSprite_;
	};
}
