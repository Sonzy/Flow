#pragma once
#include <opencv2\core.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\tracking.hpp>
#include <opencv2\core\cvstd_wrapper.hpp>

#include <thread>
#include <string>

#include "Flow\Helper\Maths.h"
#include "OpenCV\OpenCVTracker.h"

class TextureAsset;
class Sprite;

enum class ETracker
{
	Hand_Left,
	Hand_Right,
	Head
};

struct Tracker
{
	Tracker(ETracker Type, const std::string& TrackerName)
		: Tracker_Type(Type), Tracker_(new OpenCVTracker()), TrackerName_(TrackerName)
	{}

	~Tracker()
	{	delete Tracker_; }

	ETracker Tracker_Type;
	OpenCVTracker* Tracker_;
	std::string TrackerName_;
};

class OpenCVTesting
{
public:
	OpenCVTesting();
	~OpenCVTesting();
	void Initialise(bool StartTrackers = true);
	void Update();

	void DrawOpenCVControls();

	void InitialiseTrackers(ETrackingType TrackingType);
	void StartTrackers();
	void StartTracker(ETracker Tracker, IntVector2D ScreenPosition);
	void StopTrackers();
	void StopTracker(ETracker Tracker);
	void SetTrackingSize(IntVector2D TrackingSize);
	void SetTrackingSize(ETracker Tracker, IntVector2D TrackingSize);
	void SwitchTrackingMode(ETrackingType NewMode);

	bool UpdateTracking();
	void DrawTracking(ETracker Tracker);
	void RenderWindow();

	void InitialiseThreads();

	void SetFlatHeight(float Height);
	float GetHeightDeviation() const;

	const char* GetTrackingAlgorithmAsString(ETrackingType Algorithm) const;

	OpenCVTracker* GetTracker(ETracker Tracker) const;
	OpenCVTracker* GetCurrentTracker() const;
	cv::Mat* GetFrame();

	float CalculateAngle() const;

	bool Started = false;
private:

	std::vector<Tracker*> Trackers_;

	cv::Mat Frame_;
	cv::VideoCapture Capture_;

	std::string WindowName_;
	std::thread* ReadThread_;

	bool ReadData_ = true;

	IntVector2D CaptureSize_;

	ETracker CurrentTracker_;

	ETrackingType TrackingMode_ = ETrackingType::KCF;

	const char* SupportedTrackingModes_[8] = { "KCF - Recommended","Boosting","CSRT","GOTURN - NOT SUPPORTED","MedianFlow","MIL","Mosse","TLD" };
	int SelectedTrackerIndex_ = 0;

	bool ShowTrackers = false;

	float FlatHeight;

};

void OnMouseClicked(int Event, int X, int Y, int Flags, void* UserData);
