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
	Capture_.read(Frame_);

	//Start frame reading thread
	ReadThread = new std::thread([this]() {
		while (ReadData)
		{
			Capture_.read(Frame_);
		}
		FLOW_ENGINE_LOG("Thread Shutting Down");
	});

	//Initialise Tracking
	Tracker_ = cv::TrackerBoosting::create();

	//Create initial BB
	BoundingBox_ = cv::Rect2d(287, 23, 86, 320);

	///Draw bounding box
	cv::rectangle(Frame_, BoundingBox_, cv::Scalar(255, 0, 0), 2, 1);

	Tracker_->init(Frame_, BoundingBox_);
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

	{
		PROFILE_CURRENT_SCOPE("Update Tracker");

		bool Updated = Tracker_->update(Frame_, BoundingBox_);

		Updated ?
			cv::rectangle(Frame_, BoundingBox_, cv::Scalar(255, 0, 0), 2, 1) :
			cv::putText(Frame_, "Tracking failure detected", cv::Point(100, 80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 255), 2);
	}

	{
		PROFILE_CURRENT_SCOPE("FrameShow");
		cv::imshow("Window", Frame_);
	}
}

