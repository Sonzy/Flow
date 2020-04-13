#include <Flow.h>
//#include "Content\Layers\Example Layer.h"
#include "Content\Layers\AGDLayer.h"
#include "Flow\Assets\AssetSystem.h"

#include "Flow\Assets\Materials\Mat_TexturedPhong.h"

class Sandbox : public Flow::Application
{
public:
	Sandbox(const std::string& AppName)
		: Application(AppName)
	{
		/* Load assets for this app*/

		Flow::AssetSystem::LoadAsset("CharacterPlane", "Sandbox/Assets/Models/Plane.obj");
		Flow::AssetSystem::LoadAsset("CharacterPlaneTexture", "Sandbox/Assets/Textures/PlaneUVAttempt1.png");
		Flow::AssetSystem::LoadAsset("Wabble_Wood", "Flow/Assets/Textures/Wabble_Wood.png"); 

		Flow::AssetSystem::CreateMaterial<Flow::Mat_TexturedPhong>("Mat_Wood");
		Flow::Mat_TexturedPhong* WoodMat = static_cast<Flow::Mat_TexturedPhong*>(Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wood")->GetMaterial());
		WoodMat->SetTexture("Wabble_Wood");
		WoodMat->SetPixelShader("TexturedPhongPS");
		WoodMat->SetVertexShader("TexturedPhongVS");

		//ExampleLayer* Example = new ExampleLayer();
		//PushLayer(Example);

		AGDLayer* Layer = new AGDLayer();
		PushLayer(Layer);



	}

	~Sandbox()
	{
		

	}
};


Flow::Application* Flow::CreateApplication()
{
	Sandbox* App = new Sandbox("Sandbox");
	return App;
}