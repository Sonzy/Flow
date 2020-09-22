#include <Flow.h>
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Materials\Mat_TexturedPhong.h"
#include "Flow\Utils\Profiling.h"

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

		AssetSystem::LoadAsset("CharacterPlane", "Assets/Models/Plane.obj");
		AssetSystem::LoadAsset("CharacterPlaneTexture", "Assets/Textures/PlaneUVAttempt1.png");
		AssetSystem::LoadEditorAsset("Wabble_Wood", "Assets/Textures/Wabble_Wood.png");

		AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Wood");
		Mat_TexturedPhong* WoodMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_Wood")->GetMaterial());
		WoodMat->SetTexture("Wabble_Wood");
		WoodMat->SetPixelShader("TexturedPhongPS");
		WoodMat->SetVertexShader("TexturedPhongVS");
	}
};


Application* CreateApplication()
{
	Sandbox* App = new Sandbox("Sandbox");
	return App;
}