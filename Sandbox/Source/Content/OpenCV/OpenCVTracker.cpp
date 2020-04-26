#include "OpenCVTracker.h"
#include <opencv2\tracking.hpp>
#include "Flow\Logging\Log.h"
#include "Flow\Helper\Instrumentation.h"
#include "Flow\Helper\HelperMacros.h"

#define CV_GREEN cv::Scalar(0, 255, 0)
#define CV_RED cv::Scalar(0, 0, 255)
#define CV_GREY cv::Scalar(50, 50, 50)

OpenCVTracker::OpenCVTracker()
{
}

void OpenCVTracker::InitialiseTracker(ETrackingType TrackingType, IntVector2D TrackingSize)
{
	if (Tracker_)
		Tracker_.release();

	switch (TrackingType)
	{
	case ETrackingType::KCF:
		Tracker_ = cv::TrackerKCF::create();
		break;
	case ETrackingType::Boosting:
		Tracker_ = cv::TrackerBoosting::create();
		break;
	case ETrackingType::CSRT:
		Tracker_ = cv::TrackerCSRT::create();
		break;
	case ETrackingType::GOTURN:
		Tracker_ = cv::TrackerGOTURN::create();
		break;
	case ETrackingType::MedianFlow:
		Tracker_ = cv::TrackerMedianFlow::create();
		break;
	case ETrackingType::MIL:
		Tracker_ = cv::TrackerMIL::create();
		break;
	case ETrackingType::Mosse:
		Tracker_ = cv::TrackerMOSSE::create();
		break;
	case ETrackingType::TLD:
		Tracker_ = cv::TrackerTLD::create();
		break;
	default:
		return;
	}

	TrackingSize_ = TrackingSize;
	TrackingMode_ = TrackingType;
}

void OpenCVTracker::StartTracking(cv::Mat* Frame, IntVector2D ScreenPosition)
{
	CHECK_RETURN(!Tracker_, "OpenCVTracker::StartTracking: Tracker was null, initialisation may have failed.");

	if (TrackerRunning_)
	{
		FLOW_LOG("OpenCVTesting::StartTracking: Tracking in progress, cancelling...");
		StopTracking();
	}

	InitialiseTracker(TrackingMode_, TrackingSize_);

	//Create the bounding box centred on the screen position
	BoundingBox_.x = (double)ScreenPosition.X - ((double)TrackingSize_.X / 2.0);
	BoundingBox_.y = (double)ScreenPosition.Y - ((double)TrackingSize_.Y / 2.0);
	BoundingBox_.width = TrackingSize_.X;
	BoundingBox_.height = TrackingSize_.Y;


	if (!Tracker_->init(*Frame, BoundingBox_))
		FLOW_ERROR("Failed to initialise tracker");

	TrackerRunning_ = true;
	LastStartPosition_ = ScreenPosition;

	FLOW_LOG("Tracking Started. Location: {0}, {1}  Size: {2}, {3}",
		ScreenPosition.X, ScreenPosition.Y, TrackingSize_.X, TrackingSize_.Y);
}

void OpenCVTracker::StartTracking(cv::Mat* Frame)
{
	StartTracking(Frame, LastStartPosition_);
}

void OpenCVTracker::StopTracking()
{
	TrackerRunning_ = false;

	//BoundingBox_ = cv::Rect2d(0, 0, TrackingSize_.X, TrackingSize_.Y);

	FLOW_LOG("Tracking Stopped");
}

void OpenCVTracker::SetTrackingSize(IntVector2D TrackingSize)
{
	TrackingSize_ = TrackingSize;
}

bool OpenCVTracker::UpdateTracking(cv::Mat* const Frame)
{
	//PROFILE_FUNCTION(); //Pain to read through since new threads are created each frame
							//instead of a thread pool
	if (!TrackerRunning_)
	{
		TrackingTarget_ = false;
		return true;
	}

	TrackingTarget_ = Tracker_->update(*Frame, BoundingBox_);
	return TrackingTarget_;
}

void OpenCVTracker::DrawTracking(cv::Mat* Frame)
{
	PROFILE_FUNCTION();

	if (!TrackerRunning_)
	{
		cv::rectangle(*Frame, BoundingBox_, CV_GREY, 2, 1);
		return;
	}

	TrackingTarget_ ?
		cv::rectangle(*Frame, BoundingBox_, CV_GREEN , 2, 1) :
		cv::rectangle(*Frame, BoundingBox_, CV_RED , 2, 1);
}

float OpenCVTracker::GetPositionX() const
{
	return (float)BoundingBox_.x - ((float)TrackingSize_.X / 2.0f);
}

float OpenCVTracker::GetPositionY() const
{
	return (float)BoundingBox_.y - ((float)TrackingSize_.Y / 2.0f);
}

Vector2D OpenCVTracker::GetPosition() const
{
	return Vector2D(
		(float)BoundingBox_.x - ((float)TrackingSize_.X / 2.0f),
		(float)BoundingBox_.y - ((float)TrackingSize_.Y / 2.0f)
	);
}

Vector2D OpenCVTracker::GetCentre() const
{
	return Vector2D(
		(float)BoundingBox_.x + ((float)TrackingSize_.X / 2.0f),
		(float)BoundingBox_.y + ((float)TrackingSize_.Y / 2.0f)
	);
}
