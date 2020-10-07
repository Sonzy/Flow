#include "Flowpch.h"
#include "World.h"
#include "Actor.h"

#include "Flow\Application.h"

#include "Bullet\LinearMath\btIDebugDraw.h"

#include "Flow\GameFramework\Other\Skybox.h"

#include "Flow\GameFramework\Controllers\Controller.h"
#include "Flow\GameFramework\Controllers\PlayerController.h"

#if WITH_EDITOR
#include "Flow/Editor/EditorCamera.h"
#include "Flow/Editor/Editor.h"
#include "Flow/Editor/Inspector.h"
#include "Flow/Editor/SelectionGizmo.h"
#endif

LineBatcher World::sm_LineBatcher = LineBatcher();

World::World()
	: World("Unnamed World")
{	}

World::World(const std::string& WorldName)
	: m_WorldName(WorldName), m_MainLevel(new Level("Main Level")), 
#if WITH_EDITOR
	m_WorldState(WorldState::Editor)
#else
	m_WorldState(WorldState::Paused)
#endif
{
	m_DebugDrawer.Init();
	sm_LineBatcher.Initialise();
}

World::~World()
{
}

void World::SaveLevel()
{
	std::ofstream OutStream = std::ofstream("Saved/SaveFile.flvl", std::ios::out | std::ios::trunc | std::ios::binary);
	m_MainLevel->Save(OutStream);
	OutStream.close();
}

void World::LoadLevel()
{
	std::ifstream InStream = std::ifstream("Saved/SaveFile.flvl", std::ios::in | std::ios::binary);

	//Clear Physics World State
	delete m_CollisionConfig;
	delete m_Dispatcher;
	delete m_Solver;
	delete m_PhysicsWorld;
	delete m_OverlappingPairCache;
	InitialisePhysics();

	m_MainLevel->Load(InStream);

	//TODO: Gravity has to be set after object are added to physics world
	m_PhysicsWorld->setGravity(btVector3(0, -9.81f, 0));

	InStream.close();

#if WITH_EDITOR
	//Reinitialise all actors for editor
	for (auto& Act : m_MainLevel->GetActors())
	{
		Act->EditorBeginPlay();
	}
#endif
}

void World::SavePlayState()
{
	std::ofstream OutStream = std::ofstream("Saved/PlayState.flvl", std::ios::out | std::ios::trunc | std::ios::binary);
	m_MainLevel->Save(OutStream);
	OutStream.close();
}

void World::LoadPlayState()
{
	std::ifstream InStream = std::ifstream("Saved/PlayState.flvl", std::ios::in | std::ios::binary);

	// Remove all collision objects from the world then delete and restart the physics world
	for (int i = m_PhysicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = m_PhysicsWorld->getCollisionObjectArray()[i];
		m_PhysicsWorld->removeCollisionObject(obj);


	}

	//TODO:
	//Editor::GetEditor()->GetInspector()->GetSelector()->Reset();

	m_MainLevel->Load(InStream);

	InStream.close();

#if WITH_EDITOR
	//Reinitialise all actors for editor
	for (auto& Act : m_MainLevel->GetActors())
	{
		Act->EditorBeginPlay();
	}
#endif
}

void World::Render()
{
	PROFILE_FUNCTION();

	int Count = 0;


	//Light->BindLight(RenderCommand::GetMainCamera()->GetViewMatrix());

	//TODO: Move this stuff
	for (auto& Actor : m_MainLevel->GetActors())
	{
		Actor->Render();
	}


}

void World::InitialiseWorld()
{
#if WITH_EDITOR
	m_EditorCam = std::make_shared<EditorCamera>();
	RenderCommand::SetMainCamera(std::shared_ptr<EditorCamera>(m_EditorCam));
#endif

	InitialisePhysics();

	std::shared_ptr<PlayerController> NewLocalController = SpawnActor<PlayerController>("NewLocalController");
	m_RegisteredControllers.push_back(NewLocalController);
}

void World::StartEditor()
{
	m_WorldState = WorldState::Editor;
	m_MainLevel->DispatchEditorBeginPlay();
}

void World::AddDefaultInitialisedActor(Actor* NewActor)
{
	auto Ownership = std::shared_ptr<Actor>(NewActor);
	AddDefaultInitialisedActor(Ownership);
}

void World::AddDefaultInitialisedActor(std::shared_ptr<Actor> NewActor)
{
	if (!NewActor)
	{
		FLOW_ENGINE_ERROR("World::AddDefaultInitialisedActor: Actor was nullptr");
		return;
	}
	m_MainLevel->GetActors().push_back(NewActor);
}

void World::StartGame()
{
	m_WorldState = WorldState::InGame;
	FLOW_ENGINE_LOG("World State set to InGame");

	m_MainLevel->InitialiseTickList();
	m_MainLevel->DispatchBeginPlay();

	for (int i = m_PhysicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = m_PhysicsWorld->getCollisionObjectArray()[i];
		obj->activate(true);
	}


#if WITH_EDITOR
	//TODO: Dont want to do this normally, only if we want to simulate link unreal etc
	//Move camera to the editor camera
	RenderCommand::GetMainCamera()->MoveCamera(m_EditorCam->GetCameraTransform());
#endif
}

void World::PauseGame()
{
	m_WorldState = WorldState::Paused;
}

void World::StopGame()
{
#if WITH_EDITOR
	m_WorldState = WorldState::Editor;
	FLOW_ENGINE_LOG("World State set to Editor");
#endif
	// Quit here? should be editor only tbh, might make the whole func editor only
}

LineBatcher& World::GetLineBatcher_S()
{
	return Application::GetWorld()->sm_LineBatcher;
}

void World::InitialisePhysics()
{
	/* Create the physics world with default settings */
	m_CollisionConfig = new btDefaultCollisionConfiguration();
	m_Dispatcher = new btCollisionDispatcher(m_CollisionConfig);
	m_OverlappingPairCache = new btDbvtBroadphase();
	m_Solver = new btSequentialImpulseConstraintSolver;
	m_PhysicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher, m_OverlappingPairCache, m_Solver, m_CollisionConfig);

	/* Initialise Physics world properties */
	m_PhysicsWorld->setGravity(btVector3(0, -9.81f, 0));

	m_PhysicsWorld->setDebugDrawer(&m_DebugDrawer);
	m_PhysicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
}

void World::DestroyActor(std::shared_ptr<Actor> Act)
{
	if(!Act)
	{
		FLOW_ENGINE_ERROR("World::DestroyActor: Tried to delete nullptr actor");
		return;
	}

	auto& ActorArry = m_MainLevel->GetActors();
	auto Iterator = std::find(ActorArry.begin(), ActorArry.end(), Act);

	if (Iterator == ActorArry.end())
	{
		FLOW_ENGINE_ERROR("World::DestroyActor: Tried to delete actor but couldnt find it in the array {0}", Act->GetName());
		return;
	}

	ActorArry.erase(Iterator);

	//TODO: actually need to destroy all actor stuff
}

void World::DestroyActor(Actor* Act)
{
	if (!Act)
	{
		FLOW_ENGINE_ERROR("World::DestroyActor: Tried to delete nullptr actor");
		return;
	}

	auto& ActorArry = m_MainLevel->GetActors();
	auto Iterator = std::find_if(ActorArry.begin(), ActorArry.end(), [&Act](const std::shared_ptr<Actor>& Actor) { return Actor.get() == Act; });

	if (Iterator == ActorArry.end())
	{
		FLOW_ENGINE_ERROR("World::DestroyActor: Tried to delete actor but couldnt find it in the array {0}", Act->GetName());
		return;
	}

	ActorArry.erase(Iterator);
}

void World::Tick(float DeltaTime)
{
	{
		PROFILE_CURRENT_SCOPE("Update Physics World");

		if(m_WorldState == WorldState::InGame)
			m_PhysicsWorld->stepSimulation(DeltaTime, 0);
	}

	{

#if WITH_EDITOR
		if(m_EditorCam)
		m_EditorCam->Update(DeltaTime);
#endif

		PROFILE_CURRENT_SCOPE("Tick Objects");
		m_MainLevel->Tick(DeltaTime);
	}
}
const std::string& World::GetName()
{
	return m_WorldName;
}

btCollisionWorld::ClosestRayResultCallback World::WorldTrace(Vector3 Start, Vector3 End)
{
	btCollisionWorld::ClosestRayResultCallback Result(Start, End);
	Application::GetWorld()->m_PhysicsWorld->rayTest(Start, End, Result);

	return Result;
}

btDiscreteDynamicsWorld* World::GetPhysicsWorld()
{
	return Application::GetWorld()->m_PhysicsWorld;
}

World* World::Get()
{
	return Application::GetWorld();
}

void World::AddPhysicsObject(btRigidBody* Obj)
{
	m_PhysicsWorld->addRigidBody(Obj);
}

void World::AddCollisionObject(btCollisionObject* Obj)
{
	m_PhysicsWorld->addCollisionObject(Obj);
}

void World::RegisterController(std::shared_ptr<Controller> NewController)
{
	CHECK_RETURN(!NewController, "Layer::DeRegisterController: Tried to register null controller");
	m_RegisteredControllers.push_back(NewController);
}

void World::DeRegisterController(std::shared_ptr<Controller> OldController)
{
	CHECK_RETURN(!OldController, "Layer::DeRegisterController: Tried to deregister null controller");

	auto FoundIterator = std::find(m_RegisteredControllers.begin(), m_RegisteredControllers.end(), OldController);

	if (FoundIterator == m_RegisteredControllers.end())
		FLOW_ENGINE_WARNING("Tried to remove controller ");

	m_RegisteredControllers.erase(FoundIterator);
}

Controller* World::GetLocalController() const
{
	for (auto& Controller : m_RegisteredControllers)
	{
		if (Controller->IsLocalController())
			return Controller.get();
	}

	return nullptr;
}