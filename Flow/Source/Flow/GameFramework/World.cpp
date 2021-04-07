#include "pch.h"
#include "World.h"
#include "Actor.h"

#include "Application.h"

#include "Bullet\LinearMath\btIDebugDraw.h"

#include "GameFramework\Other\Skybox.h"

#include "GameFramework\Controllers\Controller.h"
#include "GameFramework\Controllers\PlayerController.h"

#include "Framework/Utils/DebugDraw.h"

#if WITH_EDITOR
#include "Editor/EditorCamera.h"
#include "Editor/Editor.h"
#include "Editor/UIComponents/Inspector.h"
#include "Editor/IconManager.h"
#include "Editor/Tools/SelectionTool.h"
#endif

#include "Framework/Utils/GUIDGenerator.h"
#include "Framework/Utils/ComponentHelper.h"
#include "Assets/AssetSystem.h"

#include <yaml-cpp/yaml.h>

LineBatcher World::sm_LineBatcher = LineBatcher();

World::World()
	: World("Unnamed World")
{	}

World::World(const std::string& WorldName)
	: m_WorldName(WorldName)
	, m_MainLevel(new Level("Main Level"))
	, m_LogGameObjectRegistering(false)
	, m_LogGameObjectDestruction(false)
#if WITH_EDITOR
	, m_WorldState(WorldState::Editor)
#else
	, m_WorldState(WorldState::Paused)
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
	YAML::Emitter outFile;
	m_MainLevel->Save(outFile);

	// Save to disk

	fs::path savePath = AssetSystem::GetGameAssetParentDirectory() / "Saved\\SaveFile.yaml"; //TODO: change file type
	std::ofstream OutStream = std::ofstream(savePath);
	OutStream << outFile.c_str();
	OutStream.close();

	//Check for errors
	if (outFile.GetLastError().empty())
	{
		FLOW_ENGINE_ERROR("Level saved to %s successfully", savePath.string().c_str());
	}
	else
	{
		FLOW_ENGINE_ERROR("Error saving level: %s", outFile.GetLastError().c_str());
	}

}

void World::LoadLevel()
{
	InitialisePhysics(true);

	fs::path savePath = AssetSystem::GetGameAssetParentDirectory() / "Saved\\SaveFile.yaml";
	FLOW_ENGINE_LOG("Loading level %s...", savePath.string().c_str());

	std::ifstream InStream = std::ifstream(savePath);
	if (InStream.is_open() == false)
	{
		FLOW_ENGINE_LOG("World::LoadLevel: Failed to open level save file");
		return;
	}

	std::stringstream stream;
	stream << InStream.rdbuf();

	YAML::Node data = YAML::Load(stream.str());
	if (data["LevelName"].IsDefined() == false)
	{
		FLOW_ENGINE_LOG("World::LoadLevel: Failed to read level save file");
		return;
	}

	//Get all actors
	std::vector<FGUID> actorsToDestroy;
	for (const std::pair<FGUID, Actor*>& Act : m_actorMap)
	{
		actorsToDestroy.push_back(Act.first);
	}

	//Destroy them
	for (FGUID guid : actorsToDestroy)
	{
		DestroyActor(guid);
	}

	//Clear the components that are left over for some reason
	if (const bool reportFreeComponents = true)
	{
		for (const std::pair<FGUID, Component*>& Act : m_componentMap)
		{
			FLOW_ENGINE_WARNING("Deleted free component %lu - %s", Act.second->GetGuid(), Act.second->GetName().c_str());
			actorsToDestroy.push_back(Act.first);
		}
	}
	m_componentMap.clear();

#if WITH_EDITOR

	Editor& e = Editor::Get();
	e.GetUIComponent<IconManager>()->Reset();

#endif

	m_MainLevel->Load(data);

	//====================

	//TODO: Gravity has to be set after object are added to physics world
	m_PhysicsWorld->setGravity(btVector3(0, -9.81f, 0));

	InStream.close();

	//Check for errors

	FLOW_ENGINE_LOG("Level %s loaded successfully", savePath.string().c_str());

#if WITH_EDITOR
	//Reinitialise all actors for editor
	for (std::pair<FGUID, Actor*> Act : m_actorMap)
	{
		Act.second->EditorBeginPlay();
	}
#endif
}

void World::SavePlayState()
{
	YAML::Emitter file;
	m_MainLevel->Save(file);
	std::ofstream outFile = std::ofstream("Saved/PlayState.flvl");
	outFile << file.c_str();
	outFile.close();
}

void World::LoadPlayState()
{
	// Remove all collision objects from the world then delete and restart the physics world
	for (int i = m_PhysicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = m_PhysicsWorld->getCollisionObjectArray()[i];
		m_PhysicsWorld->removeCollisionObject(obj);
	}

	//= Load File =
	std::ifstream InStream = std::ifstream("Saved/SaveFile.ylvl");
	if (InStream.is_open() == false)
	{
		return;
	}

	std::stringstream stream;
	stream << InStream.rdbuf();

	YAML::Node data = YAML::Load(stream.str());
	if (data["LevelName"].IsDefined() == false)
	{
		FLOW_ENGINE_LOG("World::LoadPlayState: Failed to read level save file");
		return;
	}

	m_MainLevel->Load(data);

	InStream.close();

	//=============

#if WITH_EDITOR
	//Reinitialise all actors for editor
	for (auto Act : m_actorMap)
	{
		Act.second->EditorBeginPlay();
	}
#endif
}

void World::Render()
{
	PROFILE_FUNCTION();

	//Light->BindLight(RenderCommand::GetMainCamera()->GetViewMatrix());
	//TODO: Move this stuff
	for (auto Act : m_actorMap)
	{
		Act.second->Render();
	}
}

void World::InitialiseWorld()
{
#if WITH_EDITOR
	m_EditorCam = new EditorCamera();
	RenderCommand::SetMainCamera(m_EditorCam);
#endif

	InitialisePhysics();
}

void World::StartEditor()
{
	if (m_WorldState != WorldState::Editor)
	{
		m_WorldState = WorldState::Editor;
		m_MainLevel->DispatchEditorBeginPlay();
	}
}

bool World::DestroyActor(FGUID guid)
{
	if (guid == -1)
	{
		FLOW_ENGINE_ERROR("World::DestroyActor: Tried to delete invalid actor guid");
		return false;
	}

	auto actorIt = m_actorMap.find(guid);
	if (actorIt == m_actorMap.end())
	{
		FLOW_ENGINE_ERROR("World::DestroyActor: Tried to find actor but had no actor with guid %d", guid);
		return false;
	}

	Actor* actor = m_actorMap[guid];

	//Remove physics
	actor->DestroyPhysics();

	//Destroy the components
	if (actor->GetRootComponent() != nullptr)
	{
		std::vector<WorldComponent*> components;
		ComponentHelper::BuildComponentArray(actor->GetRootComponent(), components);

		for (WorldComponent* component : components)
		{
			auto iterator = m_componentMap.find(component->GetGuid());
			if (iterator == m_componentMap.end())
			{
				FLOW_ENGINE_ERROR("World::DestroyActor: Failed to destroy component");
				continue;
			}

			if (m_LogGameObjectDestruction)
			{
				FLOW_ENGINE_ERROR("World::DestroyActor: Destroying Component %lu - %s", component->GetGuid(), component->GetName().c_str());
			}
			m_componentMap.erase(iterator);
			delete component;
		}
	}

	if (m_LogGameObjectDestruction)
	{
		FLOW_ENGINE_ERROR("World::DestroyActor: Destroying Actor %lu - %s", actor->GetGuid(), actor->GetName().c_str());
	}
	m_actorMap.erase(actorIt);

#if WITH_EDITOR
	Editor::Get().GetTool<SelectionTool>()->SelectComponent(nullptr);
#endif

	delete actor;

	return true;
}

bool World::DestroyComponent(FGUID guid)
{
	if (guid == -1)
	{
		FLOW_ENGINE_ERROR("World::DestroyComponent: Tried to delete invalid actor guid");
		return false;
	}

	auto it = m_componentMap.find(guid);
	if (it == m_componentMap.end())
	{
		FLOW_ENGINE_ERROR("World::DestroyComponent: Tried to find actor but had no actor with guid %d", guid);
		return false;
	}

	Component* comp = m_componentMap[guid];
	if (m_LogGameObjectDestruction)
	{
		FLOW_ENGINE_ERROR("World::DestroyActor: Destroying Component %lu - %s", comp->GetGuid(), comp->GetName().c_str());
	}
	m_componentMap.erase(it);

#if WITH_EDITOR
	Editor& e = Editor::Get();
	e.GetTool<SelectionTool>()->SelectComponent(nullptr);
	e.GetUIComponent<IconManager>()->RemoveIcon(comp->GetGuid());
#endif

	delete comp;

	return true;
}

void World::StartGame()
{
	m_WorldState = WorldState::InGame;
	FLOW_ENGINE_LOG("World State set to InGame");

	//TODO: Dont spawn one till the start?
	PlayerController* NewLocalController = SpawnActor<PlayerController>("NewLocalController");
	m_RegisteredControllers.push_back(NewLocalController);

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

	std::vector<Controller*> controllers = GetAllActorsOfType<Controller>();
	for (Controller* cont : controllers)
	{
		DestroyActor(cont);
	}
}

LineBatcher& World::GetLineBatcher_S()
{
	return Application::GetWorld()->sm_LineBatcher;
}

void World::PrintAllPhysicsObjects() const
{
	int i = m_PhysicsWorld->getNumCollisionObjects();
	FLOW_ENGINE_LOG("There are %d physics objects", i);
	for (i = i - 1; i >= 0; i--)
	{
		btCollisionObject* obj = m_PhysicsWorld->getCollisionObjectArray()[i];
	}
}

void World::InitialisePhysics(bool Force)
{
	if (m_PhysicsWorld && Force == false)
	{
		FLOW_ENGINE_LOG("World::InitialisePhysics: Physics Already Initialised");
		return;
	}

	//Clear Physics World State
	if (m_PhysicsWorld != nullptr)
	{
		btCollisionObjectArray objs = m_PhysicsWorld->getCollisionObjectArray();
		for (int i = 0; i < objs.size(); i++)
		{
			m_PhysicsWorld->removeCollisionObject(objs[i]);
		}
	}

	delete m_CollisionConfig;
	delete m_Dispatcher;
	delete m_OverlappingPairCache;
	delete m_Solver;
	delete m_PhysicsWorld;

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

	FLOW_ENGINE_LOG("World::InitialisePhysics: Physics Initialised");
}

Actor* World::FindActor(FGUID guid) const
{
	auto iterator = m_actorMap.find(guid);
	return iterator == m_actorMap.end() ? nullptr : iterator->second;
}

Component* World::FindComponent(FGUID guid) const
{
	auto iterator = m_componentMap.find(guid);
	return iterator == m_componentMap.end() ? nullptr : iterator->second;
}

void World::Tick(float DeltaTime)
{
	{
		PROFILE_CURRENT_SCOPE("Update Physics World");

		if (m_WorldState == WorldState::InGame)
		{
			m_PhysicsWorld->stepSimulation(DeltaTime, 0);
		}
	}

	{

#if WITH_EDITOR
		if (m_EditorCam)
		{
			m_EditorCam->Update(DeltaTime);
		}
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

World& World::Get()
{
	return *Application::GetWorld();
}

void World::AddPhysicsObject(btRigidBody* Obj)
{
	m_PhysicsWorld->addRigidBody(Obj);
}

void World::AddCollisionObject(btCollisionObject* Obj)
{
	m_PhysicsWorld->addCollisionObject(Obj);
}

void World::RegisterController(Controller* NewController)
{
	CHECK_RETURN(!NewController, "Layer::DeRegisterController: Tried to register null controller");
	m_RegisteredControllers.push_back(NewController);
}

void World::DeRegisterController(Controller* OldController)
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
		{
			return Controller;
		}
	}

	return nullptr;
}

void World::RegisterGameObject(GameObject* newObject)
{
	if (newObject->GetGuid() != -1)
	{
		FLOW_ENGINE_ERROR("World::RegisterGameObject: Tried to register to actor that already has a guid (%lu)", newObject->GetGuid());
		return;
	}

	FGUID newGuid = GUIDGen::Generate();
	newObject->SetGuid(newGuid);

	if (Actor* newActor = dynamic_cast<Actor*>(newObject))
	{
		m_actorMap[newGuid] = newActor;
		newActor->OnRegistered();

		if (m_LogGameObjectRegistering)
		{
			FLOW_ENGINE_LOG("Registered new actor: %lu %s", newGuid, newActor->GetName().c_str());
		}
	}
	else if (Component* newComponent = dynamic_cast<Component*>(newObject))
	{
		m_componentMap[newGuid] = newComponent;
		newComponent->OnRegistered();

		if (m_LogGameObjectRegistering)
		{
			FLOW_ENGINE_LOG("Registered new component: %lu %s", newGuid, newComponent->GetName().c_str());
		}
	}
}

void World::RegisterGameObject(GameObject* newObject, FGUID guid)
{
	std::unordered_map<FGUID, Actor*>::iterator foundActor = m_actorMap.find(guid);
	if (foundActor != m_actorMap.end())
	{
		m_actorMap.erase(foundActor);
	}

	std::unordered_map<FGUID, Component*>::iterator foundComponent = m_componentMap.find(guid);
	if (foundComponent != m_componentMap.end())
	{
		m_componentMap.erase(foundComponent);
	}

	newObject->SetGuid(guid);

	if (Actor* newActor = dynamic_cast<Actor*>(newObject))
	{
		m_actorMap[guid] = newActor;
		newActor->OnRegistered();


		if (m_LogGameObjectRegistering)
		{
			FLOW_ENGINE_LOG("Registered new actor with existing guid: %lu %s", guid, newActor->GetName().c_str());
		}
	}
	else if (Component* newComponent = dynamic_cast<Component*>(newObject))
	{
		m_componentMap[guid] = newComponent;
		newComponent->OnRegistered();

		if (m_LogGameObjectRegistering)
		{
			FLOW_ENGINE_LOG("Registered new component with existing guid: %lu %s", guid, newComponent->GetName().c_str());
		}
	}
}