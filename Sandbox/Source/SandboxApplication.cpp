#include <Flow.h>
#include "Content\Layers\Example Layer.h"
#include "Content\Layers\AGDLayer.h"
#include "Flow\Assets\AssetSystem.h"

#include "Flow\Assets\Materials\Mat_TexturedPhong.h"
#include "Flow\Assets\Materials\Mat_LitColour.h"

#include "Flow\Helper\Instrumentation.h"

class Sandbox : public Flow::Application
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
		Application::InitialiseApplication();

		PROFILE_FUNCTION();

		/* Load assets for this app*/

		Flow::AssetSystem::LoadAsset("CharacterPlane", "Sandbox/Assets/Models/Plane.obj");
		Flow::AssetSystem::LoadAsset("CharacterPlaneTexture", "Sandbox/Assets/Textures/PlaneUVAttempt1.png");
		Flow::AssetSystem::LoadAsset("Wabble_Wood", "Flow/Assets/Textures/Wabble_Wood.png");
		Flow::AssetSystem::LoadAsset("TestSprite", "Sandbox/Assets/Textures/TestSprite.png");
		Flow::AssetSystem::LoadAsset("TestSprite2", "Sandbox/Assets/Textures/Testt.png");
		Flow::AssetSystem::LoadAsset("MapTest", "Sandbox/Assets/Models/AGD_MapScaleTest3.obj");
		Flow::AssetSystem::LoadAsset("Tube", "Sandbox/Assets/Models/Tunnel.obj");

		Flow::AssetSystem::LoadAsset("PhongColourPS", "Flow/Source/Flow/Rendering/Core/Shaders/PhongColourP.cso");
		Flow::AssetSystem::LoadAsset("PhongColourVS", "Flow/Source/Flow/Rendering/Core/Shaders/PhongColourV.cso");

		Flow::AssetSystem::CreateMaterial<Flow::Mat_TexturedPhong>("Mat_Wood");
		Flow::Mat_TexturedPhong* WoodMat = static_cast<Flow::Mat_TexturedPhong*>(Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wood")->GetMaterial());
		WoodMat->SetTexture("Wabble_Wood");
		WoodMat->SetPixelShader("TexturedPhongPS");
		WoodMat->SetVertexShader("TexturedPhongVS");

		Flow::AssetSystem::CreateMaterial<Flow::Mat_LitColour>("Mat_LitGrey");
		Flow::Mat_LitColour* LitGreyMat = static_cast<Flow::Mat_LitColour*>(Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_LitGrey")->GetMaterial());
		LitGreyMat->SetPixelShader("PhongColourPS");
		LitGreyMat->SetVertexShader("PhongColourVS");
		LitGreyMat->SetColour(Vector(0.2f, 0.2f, 0.2f));

		Flow::AssetSystem::LoadAsset("SpriteShaderP", "Flow/Source/Flow/Rendering/Core/Shaders/SpritePS.cso");
		Flow::AssetSystem::LoadAsset("SpriteShaderV", "Flow/Source/Flow/Rendering/Core/Shaders/SpriteVS.cso");

		AGDLayer* Layer = new AGDLayer();
		PushLayer(Layer);

		//ExampleLayer* Example = new ExampleLayer();
		//PushLayer(Example);
	}
};


Flow::Application* Flow::CreateApplication()
{
	Sandbox* App = new Sandbox("Sandbox");
	return App;
}