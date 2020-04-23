#pragma once
#include "Flow\Helper\Maths.h"

#include <opencv2\highgui.hpp>

enum class ETrackingType : int8_t
{
	None = 0,
	KCF = 1,
	Boosting = 2,
	CSRT = 4,
	GOTURN = 8,
	MedianFlow = 16,
	MIL = 32,
	Mosse = 64,
	TLD = 128
};

namespace cv
{
	class Tracker;
}

class OpenCVTracker
{
public:
	OpenCVTracker();

	void InitialiseTracker(ETrackingType TrackingType, IntVector2D TrackingSize);

	void StartTracking(cv::Mat* Frame, IntVector2D ScreenPosition);
	void StartTracking(cv::Mat* Frame);

	void StopTracking();
	void SetTrackingSize(IntVector2D TrackingSize);

	bool UpdateTracking(cv::Mat* const Frame);

	void DrawTracking(cv::Mat* Frame);

	bool IsRunning() const { return TrackerRunning_; }
	bool HasTrackingTarget() const { return TrackingTarget_; }

	float GetPositionX() const;
	float GetPositionY() const;
	Vector2D GetPosition() const;

	float GetSamplingSizeX() const { return TrackingSize_.X; }
	float GetSamplingSizeY() const { return TrackingSize_.Y; }
	IntVector2D GetSamplingSize() const { return TrackingSize_; }
private:

	cv::Ptr<cv::Tracker> Tracker_;
	cv::Rect2d BoundingBox_;
	IntVector2D TrackingSize_;
	ETrackingType TrackingMode_ = ETrackingType::None;
	bool TrackerRunning_ = false;
	bool TrackingTarget_ = false;
	IntVector2D LastStartPosition_;
};