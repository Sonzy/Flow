#include <Flow.h>
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Materials\Mat_TexturedPhong.h"
#include "Framework\Utils\Profiling.h"

class Sandbox : public Application
{
public:
	Sandbox(const std::string& AppName)
		: Application(AppName)
	{

	}

	~Sandbox()
	{
		

	}

	virtual void InitialiseApplication() override
	{
		/* Initialse Engine */
		Application::InitialiseApplication();

		PROFILE_FUNCTION();

		/* Load assets for this app*/

		AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Wood");
		Mat_TexturedPhong* WoodMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_Wood")->GetMaterial());
		WoodMat->SetTexture("Wabble_Wood");
		WoodMat->SetPixelShader("TexturedPhong_PS");
		WoodMat->SetVertexShader("TexturedPhong_VS");
	}
};


Application* CreateApplication()
{
	Sandbox* App = new Sandbox("Sandbox");
	return App;
}