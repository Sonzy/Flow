#include "Flowpch.h"
#include "World.h"
#include "Actor.h"

#include "Flow\Application.h"

#include "ThirdParty\Bullet\LinearMath\btIDebugDraw.h"

#include "Flow\GameFramework\Other\Skybox.h"

#include "Flow\GameFramework\Controllers\Controller.h"
#include "Flow\GameFramework\Controllers\PlayerController.h"

#if WITH_EDITOR
#include "Flow/Editor/EditorCamera.h"
#endif

LineBatcher World::s_LineBatcher = LineBatcher();

World::World()
	: World("Unnamed World")
{	}

World::World(const std::string& WorldName)
	: _WorldName(WorldName), _MainLevel(new Level("Main Level")), 
#if WITH_EDITOR
	_WorldState(WorldState::Editor)
#else
	_WorldState(WorldState::Paused)
#endif
{
	_DebugDrawer.Init();
	s_LineBatcher.Initialise();
}

World::~World()
{
}

void World::Save()
{
	////Create File - Overwrite existing
	//std::ofstream OutStream = std::ofstream("Saved/SaveFile.flvl", std::ios::out | std::ios::trunc | std::ios::binary);
	//if (!OutStream)
	//{
	//	FLOW_ENGINE_LOG("Failed to create save file");
	//	return;
	//}
	//
	//for (auto Object : _Actors)
	//{
	//	const std::string& Name = Object->GetName();
	//
	//	OutStream.write((char*)&Name, sizeof(Name));
	//}
	//
	//OutStream.close();
}

void World::Load()
{
	//std::ifstream InStream = std::ifstream("Saved/SaveFile.flvl", std::ios::in | std::ios::binary);
	//if (!InStream)
	//{
	//	FLOW_ENGINE_LOG("Failed to load save file");
	//	return;
	//}
	//
	//char* StringBuffer = new char[sizeof(std::string)];
	//auto Position = InStream.tellg();
	//while (!InStream.eof())
	//{
	//	InStream.read(StringBuffer, sizeof(std::string));
	//	FLOW_ENGINE_LOG("Output: {0}", (std::string)StringBuffer); //TODO: Fix this shiz
	//}
	//
	//delete StringBuffer;
}

void World::InitialiseWorld()
{
#if WITH_EDITOR
	_EditorCam = std::make_shared<EditorCamera>();
	RenderCommand::SetMainCamera(std::shared_ptr<EditorCamera>(_EditorCam));
#endif

	InitialisePhysics();

	std::shared_ptr<PlayerController> NewLocalController = SpawnActor<PlayerController>("NewLocalController");
	_RegisteredControllers.push_back(NewLocalController);
}

void World::StartEditor()
{
	_WorldState = WorldState::Editor;
	_MainLevel->DispatchEditorBeginPlay();
}

void World::StartGame()
{
	_WorldState = WorldState::InGame;
	_MainLevel->InitialiseTickList();
	_MainLevel->DispatchBeginPlay();


#if WITH_EDITOR
	//TODO: Dont want to do this normally, only if we want to simulate link unreal etc
	//Move camera to the editor camera
	RenderCommand::GetMainCamera()->MoveCamera(_EditorCam->GetCameraTransform());
#endif
}

void World::PauseGame()
{
	_WorldState = WorldState::Paused;
}

void World::StopGame()
{
#if WITH_EDITOR
	_WorldState = WorldState::Editor;
#endif
	// Quit here? should be editor only tbh, might make the whole func editor only
}

LineBatcher& World::GetLineBatcher_S()
{
	return Application::GetWorld()->s_LineBatcher;
}

void World::InitialisePhysics()
{
	/* Create the physics world with default settings */
	_CollisionConfig = new btDefaultCollisionConfiguration();
	_Dispatcher = new btCollisionDispatcher(_CollisionConfig);
	_OverlappingPairCache = new btDbvtBroadphase();
	_Solver = new btSequentialImpulseConstraintSolver;
	_PhysicsWorld = new btDiscreteDynamicsWorld(_Dispatcher, _OverlappingPairCache, _Solver, _CollisionConfig);

	/* Initialise Physics world properties */
	_PhysicsWorld->setGravity(btVector3(0, -9.81, 0));

	_PhysicsWorld->setDebugDrawer(&_DebugDrawer);
	_PhysicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
}

void World::Tick(float DeltaTime)
{
	{
		PROFILE_CURRENT_SCOPE("Update Physics World");

		if(_WorldState == WorldState::InGame)
			_PhysicsWorld->stepSimulation(DeltaTime, 0);
	}

	{

#if WITH_EDITOR
		if(_EditorCam)
		_EditorCam->Update(DeltaTime);
#endif

		PROFILE_CURRENT_SCOPE("Tick Objects");
		_MainLevel->Tick(DeltaTime);
	}
}
const std::string& World::GetName()
{
	return _WorldName;
}

btCollisionWorld::ClosestRayResultCallback World::WorldTrace(Vector Start, Vector End)
{
	btVector3 InternalStart = btVector3(Start.X, Start.Y, Start.Z);
	btVector3 InternaEnd = btVector3(End.X, End.Y, End.Z);

	btCollisionWorld::ClosestRayResultCallback Result(InternalStart, InternaEnd);
	Application::GetWorld()->_PhysicsWorld->rayTest(InternalStart, InternaEnd, Result);

	return Result;
}

btDiscreteDynamicsWorld* World::GetPhysicsWorld()
{
	return Application::GetWorld()->_PhysicsWorld;
}

World* World::GetWorld()
{
	return Application::GetWorld();
}

void World::AddPhysicsObject(btRigidBody* Obj)
{
	_PhysicsWorld->addRigidBody(Obj);
}

void World::AddCollisionObject(btCollisionObject* Obj)
{
	_PhysicsWorld->addCollisionObject(Obj);
}

void World::RegisterController(std::shared_ptr<Controller> NewController)
{
	CHECK_RETURN(!NewController, "Layer::DeRegisterController: Tried to register null controller");
	_RegisteredControllers.push_back(NewController);
}

void World::DeRegisterController(std::shared_ptr<Controller> OldController)
{
	CHECK_RETURN(!OldController, "Layer::DeRegisterController: Tried to deregister null controller");

	auto FoundIterator = std::find(_RegisteredControllers.begin(), _RegisteredControllers.end(), OldController);

	if (FoundIterator == _RegisteredControllers.end())
		FLOW_ENGINE_WARNING("Tried to remove controller ");

	_RegisteredControllers.erase(FoundIterator);
}

Controller* World::GetLocalController() const
{
	for (auto& Controller : _RegisteredControllers)
	{
		if (Controller->IsLocalController())
			return Controller.get();
	}

	return nullptr;
}