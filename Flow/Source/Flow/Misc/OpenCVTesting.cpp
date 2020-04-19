#include "Flowpch.h"
#include "OpenCVTesting.h"
#include "Flow\Assets\Textures\TextureAsset.h"
#include "ThirdParty\ImGui\imgui.h"

#include "Flow\Rendering\Core\Bindables\Texture.h"
#include "Flow\Assets\AssetSystem.h"

#include "Flow\Helper\Instrumentation.h"

#include <opencv2\tracking.hpp>

Flow::OpenCVTesting::OpenCVTesting()
{
	PROFILE_FUNCTION();

	Capture_.open(0, cv::CAP_MSMF);

	CHECK_RETURN(!Capture_.isOpened(), "OpenCVTesting::OpenCVTesting: Failed to open video capture");

	Capture_.read(Frame_);

	FLOW_ENGINE_LOG("API: {0}", Capture_.getBackendName());

	ReadThread = new std::thread([this]() {
		while (ReadData)
		{
			Capture_.read(Frame_);
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

	{
		PROFILE_CURRENT_SCOPE("FrameShow");
		cv::imshow("Window", Frame_);
	}
}

