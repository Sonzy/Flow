#include "Example Layer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Rendering\Core\Materials\Material.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"

#include "Content/ExampleWorldObject.h"
#include "Flow\Rendering\Core\Camera\Camera.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	for (int i = 0; i < 1; i++)
	{
		Actors.push_back(Flow::Application::GetWorld()->SpawnWorldObject<ExampleWorldObject>());
	}

	Light = std::make_shared<Flow::PointLight>(500.0f);


	///* collision configuration contains default setup for memory , collision setup .
	//Advanced users can create their own configuration . */
	//btDefaultCollisionConfiguration* CollisionConfig = new btDefaultCollisionConfiguration();
	//
	///// use the default collision dispatcher . For parallel processing you can use a diffent
	////dispatcher(see Extras / BulletMultiThreaded)
	//btCollisionDispatcher* dispatcher = new btCollisionDispatcher(CollisionConfig);
	//
	///// btDbvtBroadphase is a good general purpose broadphase . You can also try out btAxis3Sweep .
	//btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	//
	///// the default constraint solver . For parallel processing you can use a different solver (see Extras / BulletMultiThreaded)
	//btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	//
	//btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
	//	overlappingPairCache, solver, CollisionConfig);
	//
	//dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

ExampleLayer::~ExampleLayer()
{
	Actors.clear();
}

void ExampleLayer::OnUpdate(float DeltaTime)
{
	int Count = 0;
	Flow::Renderer::BeginScene();

	Light->BindLight(Flow::RenderCommand::GetCamera().GetMatrix());

	for (auto& Actor : Actors)
	{
		Actor->Render();
	}

	Flow::Renderer::EndScene();

	Flow::RenderCommand::GetCamera().Tick(DeltaTime);
}

void ExampleLayer::OnImGuiRender()
{
	Flow::RenderCommand::GetCamera().RenderIMGUIWindow();

	Flow::AssetSystem::RenderDebugWindow(true);
	Light->RenderControlWindow();
}

void ExampleLayer::OnAttach()
{
	Layer::OnAttach();
}
