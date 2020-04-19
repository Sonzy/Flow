#include "Flowpch.h"
#include "OpenCVTesting.h"
#include "Flow\Assets\Textures\TextureAsset.h"
#include "ThirdParty\ImGui\imgui.h"

#include "Flow\Rendering\Core\Bindables\Texture.h"
#include "Flow\Assets\AssetSystem.h"

#include "Flow\Helper\Instrumentation.h"

#include <opencv2\core\directx.hpp>

#include <opencv2\video\tracking.hpp>
#include <opencv2\core\ocl.hpp>

Flow::OpenCVTesting::OpenCVTesting()
	: CaptureTexture_(nullptr)
{
	PROFILE_FUNCTION();

	Capture_.open(0, cv::CAP_MSMF);
	//Capture_.open(0, cv::CAP_FFMPEG);
	//Capture_.open("TestVideo.mp4", 1000);
	CHECK_RETURN(!Capture_.isOpened(), "OpenCVTesting::OpenCVTesting: Failed to open video capture");

	//CaptureTexture_ = new TextureAsset();
	//CaptureTexture_->OverwriteAssetPath("OpenCVCamera");

	Capture_.read(Frame_);
	//CaptureTexture_->ManualInit(Frame_.cols, Frame_.rows);

	//PaddedFrame_ = cv::Mat(Frame_.size(), CV_MAKE_TYPE(Frame_.depth(), 4));

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

	//CHECK_RETURN(!Capture_.isOpened(), "OpenCVTesting::OpenCVTesting: Cannot update from capture");

	if (!Capture_.isOpened())
		return;
	//{
	//	PROFILE_CURRENT_SCOPE("FrameRead");
	//
	//	if(Capture_.grab())
	//		Capture_.read(Frame_);
	//}

	{
		PROFILE_CURRENT_SCOPE("FrameShow");
		cv::imshow("Window", Frame_);
	}

	//{
	//	PROFILE_CURRENT_SCOPE("Image Conversion");
	//
	//	//int Channels = Frame_.channels();
	//	//cv::Scalar_<uint8_t> Pixel; //BGR
	//	//for (int i = 0; i < Frame_.rows; i++)
	//	//{
	//	//	uint8_t* Row = reinterpret_cast<uint8_t*>(&Frame_.row(i)); //Might not work with updated api
	//	//	for (int j = 0; j < Frame_.cols; j++)
	//	//	{
	//	//		cv::Vec3f Vector = Frame_.at<cv::Vec3b>(i, j);
	//	//		//CaptureTexture_->AlterPixel(j, i, TexColor(Row[j * Channels + 2], Row[j * Channels + 1], Row[j * Channels + 0]));
	//	//		CaptureTexture_->AlterPixel(j, i, std::move(TexColor(Vector.val[2], Vector.val[1], Vector.val[0])));
	//	//	}
	//	//}
	//
	//	//cv::imwrite("Assets/Textures/WebcamOutput.png", Frame_);
	//	//CaptureTexture_->UpdateFromFile("Sandbox/Assets/Textures/WebcamOutput.png");
	//}
}

void Flow::OpenCVTesting::RenderToIMGUI()
{
	PROFILE_FUNCTION();

	CHECK_RETURN(!CaptureTexture_, "Capture was nullptr");
	auto Tex2 = reinterpret_cast<Texture*>(Texture::Resolve(CaptureTexture_, 0).get());

	int conversion[]{ 0,0,1,1,2,2,-1,3 };
	cv::mixChannels(&Frame_, 1, &PaddedFrame_, 1, conversion, 4);

	Tex2->Update(&PaddedFrame_);

	if (ImGui::Begin("OpenCV"))
	{
		ImGui::Image((void*)Tex2->GetTextureUnsafe(), ImVec2(CaptureTexture_->GetWidth(), CaptureTexture_->GetHeight()));
	}
	ImGui::End();
}
