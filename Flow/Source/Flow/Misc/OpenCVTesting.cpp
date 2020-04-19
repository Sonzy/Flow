#include "Flowpch.h"
#include "OpenCVTesting.h"
#include "Flow\Assets\Textures\TextureAsset.h"
#include "ThirdParty\ImGui\imgui.h"

#include "Flow\Rendering\Core\Bindables\Texture.h"
#include "Flow\Assets\AssetSystem.h"

#include "Flow\Helper\Instrumentation.h"

Flow::OpenCVTesting::OpenCVTesting()
	: CaptureTexture_(nullptr)
{
	PROFILE_FUNCTION();

	Capture_.open(0);
	CHECK_RETURN(!Capture_.isOpened(), "OpenCVTesting::OpenCVTesting: Failed to open video capture");

	CaptureTexture_ = new TextureAsset();
	CaptureTexture_->OverwriteAssetPath("OpenCVCamera");

	Capture_.read(Frame_);
	CaptureTexture_->ManualInit(Frame_.cols, Frame_.rows);
}

void Flow::OpenCVTesting::Update()
{
	PROFILE_FUNCTION();

	CHECK_RETURN(!Capture_.isOpened(), "OpenCVTesting::OpenCVTesting: Cannot update from capture");

	Capture_.read(Frame_);


	//TODO: Convert to readable format and display
	
	int Channels = Frame_.channels();
	cv::Scalar_<uint8_t> Pixel; //BGR
	for (int i = 0; i < Frame_.rows; i++)
	{
		uint8_t* Row = reinterpret_cast<uint8_t*>(&Frame_.row(i)); //Might not work with updated api
		for (int j = 0; j < Frame_.cols; j++)
		{
			cv::Vec3f Vector = Frame_.at<cv::Vec3b>(i, j);
			//CaptureTexture_->AlterPixel(j, i, TexColor(Row[j * Channels + 2], Row[j * Channels + 1], Row[j * Channels + 0]));
			CaptureTexture_->AlterPixel(j, i, TexColor(Vector.val[2], Vector.val[1], Vector.val[0]));
		}
	}	
}

void Flow::OpenCVTesting::RenderToIMGUI()
{
	PROFILE_FUNCTION();

	CHECK_RETURN(!CaptureTexture_, "Capture was nullptr");
	//auto Tex = reinterpret_cast<Texture*>(Texture::Resolve(AssetSystem::GetAsset<TextureAsset>("TestSprite"), 0).get());
	auto Tex2 = reinterpret_cast<Texture*>(Texture::Resolve(CaptureTexture_, 0).get());
	Tex2->Update(CaptureTexture_);

	if (ImGui::Begin("OpenCV"))
	{
		ImGui::Image((void*)Tex2->GetTextureUnsafe(), ImVec2(CaptureTexture_->GetWidth(), CaptureTexture_->GetHeight()));
	}
	ImGui::End();
}
