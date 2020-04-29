#include "Flowpch.h"
#include "OpenCVTesting.h"
#include "Flow\Assets\Textures\TextureAsset.h"
#include "ThirdParty\ImGui\imgui.h"

#include "Flow\Rendering\Core\Bindables\Texture.h"
#include "Flow\Assets\AssetSystem.h"

#include "Flow\Helper\Instrumentation.h"

#include "Content\Layers\AGDLayer.h"

#define CV_GREY cv::Scalar(50, 50, 50)

OpenCVTesting::OpenCVTesting()
	: ReadThread_(nullptr)
{

}

OpenCVTesting::~OpenCVTesting()
{
}

void OpenCVTesting::Initialise(bool StartTrackers)
{
	PROFILE_FUNCTION();

	//Open Capture
	Capture_.open(0, cv::CAP_ANY);

	//Check for errors
	CHECK_RETURN(!Capture_.isOpened(), "OpenCVTesting::OpenCVTesting: Failed to open video capture");
	//FLOW_LOG("API: {0}", Capture_.getBackendName());

	//Configuration
	CaptureSize_ = IntVector2D(1280, 720);

	Capture_.set(cv::CAP_PROP_FRAME_WIDTH, CaptureSize_.X);
	Capture_.set(cv::CAP_PROP_FRAME_HEIGHT, CaptureSize_.Y);

	//Create Window
	WindowName_ = "Camera Window";
	cv::namedWindow(WindowName_);
	cv::setMouseCallback(WindowName_, OnMouseClicked, nullptr);

	//Read first frame
	Capture_.read(Frame_);

	InitialiseThreads();

	//Initialise Tracking
	InitialiseTrackers(TrackingMode_);

	if (StartTrackers)
	{
		StartTracker(ETracker::Hand_Left, IntVector2D(200, 300));
		StartTracker(ETracker::Hand_Right, IntVector2D(300, 300));
		//StartTracker(ETracker::Head, IntVector2D(400, 300));
	}
}

void OpenCVTesting::Update()
{
	PROFILE_FUNCTION();

	if (!Capture_.isOpened())
		return;

	//If we haven't started, draw the template markers
	if (!Started)
	{
		cv::rectangle(Frame_, cv::Rect2d(950 - (128 / 2), 500 - (128 / 2), 128, 128), CV_GREY, 2, 1);
		cv::rectangle(Frame_, cv::Rect2d(600 - (128 / 2), 500 - (128 / 2), 128, 128), CV_GREY, 2, 1);

		RenderWindow();
		return;
	}

	UpdateTracking();

	DrawTracking(ETracker::Hand_Left);
	DrawTracking(ETracker::Hand_Right);
	//DrawTracking(ETracker::Head);



	RenderWindow();
}

void OpenCVTesting::DrawOpenCVControls()
{
	if (ImGui::Begin("Tracking Controls"))
	{
		ImGui::Text("Tracking Algorithm: %s", GetTrackingAlgorithmAsString(TrackingMode_));
		ImGui::Checkbox("Show Tracker Algorithms", &ShowTrackers);
		if (ShowTrackers)
		{
			if (ImGui::ListBox("Supported Trackers", &SelectedTrackerIndex_, SupportedTrackingModes_, IM_ARRAYSIZE(SupportedTrackingModes_)))
				SwitchTrackingMode((ETrackingType)(1 << SelectedTrackerIndex_));
		}


		if (ImGui::BeginTabBar("Enabled Trackers"))
		{
			for (auto TrackerData : Trackers_)
			{
				if (ImGui::BeginTabItem(TrackerData->TrackerName_.c_str()))
				{
					CurrentTracker_ = TrackerData->Tracker_Type;
					bool Tracking = TrackerData->Tracker_->HasTrackingTarget();
					bool Running = TrackerData->Tracker_->IsRunning();

					ImGui::TextColored(Running ? IMGUI_GREEN : IMGUI_RED, "Tracker: %s %s", 
						TrackerData->TrackerName_.c_str(), Running ? "Running" : "Stopped");
					ImGui::TextColored(
						Running ? (Tracking ? IMGUI_GREEN : IMGUI_RED) : IMGUI_RED , "Tracking Status: %s",
						Running ? (Tracking ? "Tracking" : "Lost Target" ) : "Stopped" );

					ImGui::Separator();


					ImGui::Text("Position: %f, %f", TrackerData->Tracker_->GetPositionX(), TrackerData->Tracker_->GetPositionY());
					ImGui::Text("Sampling Size: %f, %f", TrackerData->Tracker_->GetSamplingSizeX(), TrackerData->Tracker_->GetSamplingSizeY());

					ImGui::Separator();

					ImGui::TextColored(IMGUI_YELLOW, "Click on the tracking window to retarget the tracker.");

					if (Running)
					{
						if (ImGui::Button("Stop Tracking"))
						{
							TrackerData->Tracker_->StopTracking();
						}
					}
					else
					{
						if (ImGui::Button("Start Tracking"))
						{
							TrackerData->Tracker_->StartTracking(&Frame_);
						}
					}

					ImGui::EndTabItem();
				}
		
			}
			ImGui::EndTabBar();
		}

		ImGui::TextColored(IMGUI_PURPLE, "Current Angle: %f", CalculateAngle());
	}
	ImGui::End();
}

void OpenCVTesting::InitialiseTrackers(ETrackingType TrackingType)
{
	IntVector2D TrackingSize = IntVector2D(128, 128);

	Tracker* Tracker_LeftHand_ = new Tracker(ETracker::Hand_Left, "Left Hand Tracker");
	Tracker* Tracker_RightHand_ = new Tracker(ETracker::Hand_Right, "Right Hand Tracker");
	Tracker* Tracker_Head_ = new Tracker(ETracker::Head, "Head Tracker");

	Tracker_LeftHand_->Tracker_->InitialiseTracker(TrackingType, TrackingSize);
	Tracker_RightHand_->Tracker_->InitialiseTracker(TrackingType, TrackingSize);
	Tracker_Head_->Tracker_->InitialiseTracker(TrackingType, TrackingSize);

	Trackers_.push_back(Tracker_LeftHand_);
	Trackers_.push_back(Tracker_RightHand_);
	Trackers_.push_back(Tracker_Head_);
}

void OpenCVTesting::StartTrackers()
{
	for (auto TrackerData : Trackers_)
	{
		TrackerData->Tracker_->StartTracking(&Frame_);
	};
}

void OpenCVTesting::StartTracker(ETracker Tracker, IntVector2D ScreenPosition)
{
	GetTracker(Tracker)->StartTracking(&Frame_, ScreenPosition);
}

void OpenCVTesting::StopTrackers()
{
	for (auto TrackerData : Trackers_)
	{
		TrackerData->Tracker_->StopTracking();
	}
}

void OpenCVTesting::StopTracker(ETracker Tracker)
{
	GetTracker(Tracker)->StopTracking();
}

void ::OpenCVTesting::SetTrackingSize(IntVector2D TrackingSize)
{
	for (auto TrackerData : Trackers_)
	{
		TrackerData->Tracker_->SetTrackingSize(TrackingSize);
	}
}

void OpenCVTesting::SetTrackingSize(ETracker Tracker, IntVector2D TrackingSize)
{
	GetTracker(Tracker)->SetTrackingSize(TrackingSize);
}

void OpenCVTesting::SwitchTrackingMode(ETrackingType NewMode)
{
	//Not supported, dont have a trained model for this mode
	if (NewMode == ETrackingType::GOTURN)
		return;

	for (auto TrackerData : Trackers_)
	{
		TrackerData->Tracker_->InitialiseTracker(NewMode, TrackerData->Tracker_->GetSamplingSize());
		TrackerData->Tracker_->StartTracking(&Frame_);
	}

	TrackingMode_ = NewMode;
}

bool OpenCVTesting::UpdateTracking()
{
	PROFILE_FUNCTION();

	OpenCVTracker* LeftTracker = GetTracker(ETracker::Hand_Left);
	OpenCVTracker* RightTracker = GetTracker(ETracker::Hand_Right);
//	OpenCVTracker* HeadTracker = GetTracker(ETracker::Head);

	std::thread Thread_Left  = std::thread([this, LeftTracker]() mutable {  LeftTracker->UpdateTracking(&Frame_);		});
	std::thread Thread_Right = std::thread([this, RightTracker]() mutable {  RightTracker->UpdateTracking(&Frame_);	});
//	std::thread Thread_Head  = std::thread([this, HeadTracker]() mutable {  HeadTracker->UpdateTracking(&Frame_);		});

	Thread_Left.join();
	Thread_Right.join();
//	Thread_Head.join();

	return true;
}

void ::OpenCVTesting::DrawTracking(ETracker Tracker)
{
	GetTracker(Tracker)->DrawTracking(&Frame_);
}

void OpenCVTesting::RenderWindow()
{
	PROFILE_FUNCTION();

	cv::imshow(WindowName_, Frame_);
}

void OpenCVTesting::InitialiseThreads()
{
	CHECK_RETURN(ReadThread_, "OpenCVTesting::InitialiseThreads: FrameReader Thread already initialised");

	ReadThread_ = new std::thread([this]() {
		FLOW_LOG("FrameReader: Thread Started");

		while (ReadData_) 
		{
			Capture_.read(Frame_);
		}

		FLOW_LOG("FrameReader: Thread Shutting Down");
		});
}

void OpenCVTesting::SetFlatHeight(float Height)
{
	FlatHeight = Height;
}

float OpenCVTesting::GetHeightDeviation() const
{
	return 	GetTracker(ETracker::Hand_Left)->GetCentre().Y - FlatHeight;
}

const char* OpenCVTesting::GetTrackingAlgorithmAsString(ETrackingType Algorithm) const
{
	switch (Algorithm)
	{
	case ETrackingType::KCF:
		return "KCF";
	case ETrackingType::Boosting:
		return "Boosting";
	case ETrackingType::CSRT:
		return "CSRT";
	case ETrackingType::GOTURN:
		return "GOTURN";
	case ETrackingType::MedianFlow:
		return "Median Flow";
	case ETrackingType::MIL:
		return "MIL";
	case ETrackingType::Mosse:
		return "Mosse";
	case ETrackingType::TLD:
		return "TLD";
	default:
		return "ALGORITHM_RETRIEVAL_ERROR";
	}
	return "ALGORITHM_RETRIEVAL_ERROR";
}

OpenCVTracker* OpenCVTesting::GetTracker(ETracker TrackerType) const
{
	for (auto TrackerData : Trackers_)
	{
		if (TrackerData->Tracker_Type == TrackerType)
			return TrackerData->Tracker_;
	}

	return nullptr;
}

OpenCVTracker* OpenCVTesting::GetCurrentTracker() const
{
	return GetTracker(CurrentTracker_);
}

cv::Mat* OpenCVTesting::GetFrame()
{
	return &Frame_;
}

float OpenCVTesting::CalculateAngle() const
{
	if (!GetTracker(ETracker::Hand_Left) || !GetTracker(ETracker::Hand_Right))
		return 0.0f;

	Vector2D LeftPos = GetTracker(ETracker::Hand_Left)->GetPosition();
	Vector2D RightPos = GetTracker(ETracker::Hand_Right)->GetPosition();
	//Vector2D AngleVector = LeftPos - RightPos;
	Vector2D AngleVector = RightPos - LeftPos;
	double angle = atan2(AngleVector.Y, AngleVector.X);// *(180 / Math::PI_D);
	return angle;
}

void OnMouseClicked(int Event, int X, int Y, int Flags, void* UserData)
{
	if (Event == cv::EVENT_LBUTTONDOWN)
	{
		OpenCVTesting* Tester = AGDLayer::GetCVTester();
		if (Tester->GetCurrentTracker()->IsRunning())
			Tester->GetCurrentTracker()->StartTracking(Tester->GetFrame(), IntVector2D(X, Y));
	}
}
