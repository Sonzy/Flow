#pragma once
#include "Flow\Core.h"

#include <opencv2\core.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\tracking.hpp>
#include <opencv2\core\cvstd_wrapper.hpp>

#include <thread>

#include "Flow\Helper\Timer.h"

namespace Flow
{
	class TextureAsset;
	class Sprite;

	class FLOW_API OpenCVTesting
	{
	public:
		OpenCVTesting();
		~OpenCVTesting();
		void Update();

		cv::Mat Frame_;
		cv::VideoCapture Capture_;
		cv::Ptr<cv::Tracker> Tracker_;
		cv::Rect2d BoundingBox_;
		
		std::thread* ReadThread;
		bool ReadData = true;
		bool SuccessRead = false;

		Flow::Timer ReadTimer;
		float FPS = 30.0f;
	};
}
