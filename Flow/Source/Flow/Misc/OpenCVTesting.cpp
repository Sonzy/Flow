#include "Flowpch.h"
#include "OpenCVTesting.h"
#include "Flow\Assets\Textures\TextureAsset.h"
#include "ThirdParty\ImGui\imgui.h"

#include "Flow\Rendering\Core\Bindables\Texture.h"
#include "Flow\Assets\AssetSystem.h"

#include "Flow\Helper\Instrumentation.h"


Flow::OpenCVTesting::OpenCVTesting()
{
	PROFILE_FUNCTION();

	//Open Capture
	Capture_.open(0, cv::CAP_MSMF);

	//Verify Capture
	CHECK_RETURN(!Capture_.isOpened(), "OpenCVTesting::OpenCVTesting: Failed to open video capture");
	FLOW_ENGINE_LOG("API: {0}", Capture_.getBackendName());

	//Read first frame
	bool FirstCheck = Capture_.read(Frame_);

	if (!FirstCheck)
		FLOW_ENGINE_ERROR("Failed to read video");

	//Initialise Tracking
	Tracker_ = cv::TrackerKCF::create();

	//Create initial BB
	BoundingBox_ = cv::Rect2d(50, 170, 128, 128);

	///Draw bounding box
	cv::rectangle(Frame_, BoundingBox_, cv::Scalar(255, 0, 0), 2, 1);

	Tracker_->init(Frame_, BoundingBox_);

	ReadTimer.Mark();

	//Start frame reading thread
	ReadThread = new std::thread([this]() {
		while (ReadData)
		{
			if (ReadTimer.Peek() > 1.0f / FPS)
			{
				SuccessRead = Capture_.read(Frame_);
				ReadTimer.Mark();
			}

		}
		FLOW_ENGINE_LOG("Thread Shutting Down");
		});
}

Flow::OpenCVTesting::~OpenCVTesting()
{
	ReadData = false;
}

void Flow::OpenCVTesting::Update()
{
	PROFILE_FUNCTION();

	if (!Capture_.isOpened())
		return;

	if (Frame_.empty())
		return;

	bool Updated;
	{
		PROFILE_CURRENT_SCOPE("Update Tracker");

		Updated = Tracker_->update(Frame_, BoundingBox_);

	}

	{
		PROFILE_CURRENT_SCOPE("Draw Results");

		Updated ?
			cv::rectangle(Frame_, BoundingBox_, cv::Scalar(0, 255, 0), 2, 1) :
			cv::rectangle(Frame_, BoundingBox_, cv::Scalar(0, 0, 255), 2, 1);
	}

	{
		PROFILE_CURRENT_SCOPE("FrameShow");
		cv::imshow("Window", Frame_);
	}
}

