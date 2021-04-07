#include "pch.h"
#include "Spawner.h"
#include "ThirdParty/ImGui/imgui.h"
#include "GameFramework/World.h"

#include "GameFramework/Actors/StaticMeshActor.h"
#include "GameFramework/Actors/PointLightActor.h"
#include "GameFramework/Actors/DirectionalLightActor.h"
#include "GameFramework/Actors/SkyboxActor.h"
#include "GameFramework/Actors/CameraActor.h"
#include "GameFramework/Actors/SpriteActor.h"

#include "GameFramework/Components/StaticMeshComponent.h"
#include "GameFramework/Components/SpriteComponent.h"
#include "GameFramework/Components/Lights/PointLightComponent.h"
#include "GameFramework/Components/Lights/DirectionalLightComponent.h"
#include "GameFramework/Components/SkyboxComponent.h"
#include "GameFramework/Components/CameraComponent.h"
#include "GameFramework/Other/ClassFactory.h"

#include "Framework/Utils/ComponentHelper.h"

#include "Editor/Editor.h"

Spawner::Spawner()
	: m_WorldReference(World::Get())
{
	m_SpawnDistance = 20.0f;

	RegisterActorClass<StaticMeshActor>("Static Mesh Actor");
	RegisterActorClass<PointLightActor>("Point Light Actor");
	RegisterActorClass<DirectionalLightActor>("Directional Light Actor");
	RegisterActorClass<SkyboxActor>("Skybox Actor");
	RegisterActorClass<CameraActor>("Camera Actor");
	RegisterActorClass<SpriteActor>("Sprite Actor");

	RegisterComponentClass<WorldComponent>("World Component");
	RegisterComponentClass<StaticMeshComponent>("Static Mesh Component");
	RegisterComponentClass<SkyboxComponent>("Skybox Component");
	RegisterComponentClass<SpriteComponent>("Sprite Component");
	RegisterComponentClass<PointLightComponent>("Point Light Component");
	RegisterComponentClass<DirectionalLightComponent>("Directional Light Component");
}

void Spawner::Update()
{

}

void Spawner::DrawComponentSpawnContextWindow(Actor* Parent)
{
	if (ImGui::BeginMenu("Add Component"))
	{
		WorldComponent* NewComponent = nullptr;

		for (auto& componentClass : m_componentClassMap)
		{
			if (ImGui::MenuItem(componentClass.second.c_str()))
			{
				NewComponent = ClassFactory::Get().CreateObjectFromID<WorldComponent>(componentClass.first);
				NewComponent->DefaultInitialise();
			}
		}

		if (NewComponent != nullptr)
		{
			NewComponent->SetParent(Parent);

			if (WorldComponent* Root = Parent->GetRootComponent())
			{
				Root->AddChild(NewComponent);
			}
			else
			{
				Parent->SetRootComponent(NewComponent);
			}

			World::Get().RegisterGameObject(NewComponent);
		}

		ImGui::EndMenu();
	}
}

void Spawner::DrawActorSpawnContextWindow()
{
	if (ImGui::BeginMenu("Spawn Actor"))
	{
		World& world = World::Get();
		Actor* newActor = nullptr;

		for (auto& ActorClass : m_actorClassMap)
		{
			if (ImGui::MenuItem(ActorClass.second.c_str()) && !newActor)
			{
				newActor = ClassFactory::Get().CreateObjectFromID<Actor>(ActorClass.first);
				World::Get().RegisterGameObject(newActor);
			}
		}

		// Initialise the spawned actor if we spawned one
		if (newActor)
		{
			//TODO: Not working right
			Transform CameraTrans = RenderCommand::GetMainCamera()->GetCameraTransform();
			newActor->GetRootComponent()->SetWorldPosition(CameraTrans.m_Position + (CameraTrans.m_Rotation.GetForwardVector() * m_SpawnDistance));

			switch (m_WorldReference.GetWorldState())
			{
			case WorldState::Editor: newActor->EditorBeginPlay(); break;
			case WorldState::Paused:
			case WorldState::InGame: newActor->BeginPlay(); break;
			}
		}

		ImGui::EndMenu();
	}
}
