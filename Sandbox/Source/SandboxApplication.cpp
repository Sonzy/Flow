#include <Flow.h>
#include "Content\Layers\Example Layer.h"
#include "Flow\Assets\AssetSystem.h"

#include "Flow\Assets\Materials\Mat_TexturedPhong.h"

class Sandbox : public Application
{
public:
	Sandbox(const std::string& AppName)
		: Application(AppName)
	{
		/* Load assets for this app*/

		AssetSystem::LoadAsset("CharacterPlane", "Sandbox/Assets/Models/Plane.obj");
		AssetSystem::LoadAsset("CharacterPlaneTexture", "Sandbox/Assets/Textures/PlaneUVAttempt1.png");
		AssetSystem::LoadAsset("Wabble_Wood", "Flow/Assets/Textures/Wabble_Wood.png"); 

		AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Wood");
		Mat_TexturedPhong* WoodMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_Wood")->GetMaterial());
		WoodMat->SetTexture("Wabble_Wood");
		WoodMat->SetPixelShader("TexturedPhongPS");
		WoodMat->SetVertexShader("TexturedPhongVS");

		ExampleLayer* Example = new ExampleLayer();
		PushLayer(Example);
	}

	~Sandbox()
	{
		

	}
};


Application* CreateApplication()
{
	Sandbox* App = new Sandbox("Sandbox");
	return App;
}