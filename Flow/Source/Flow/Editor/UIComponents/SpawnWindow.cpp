#include "Flowpch.h"
#include "SpawnWindow.h"
#include "ThirdParty/ImGui/imgui.h"
#include "GameFramework/World.h"

#include "GameFramework/Actors/StaticMeshActor.h"
#include "GameFramework/Actors/PointLightActor.h"
#include "GameFramework/Actors/SkyboxActor.h"
#include "GameFramework/Actors/CameraActor.h"
#include "GameFramework/Actors/SpriteActor.h"

#include "GameFramework/Components/StaticMeshComponent.h"
#include "GameFramework/Components/SpriteComponent.h"
#include "GameFramework/Components/PointLightComponent.h"
#include "GameFramework/Components/CameraComponent.h"
#include "GameFramework/Other/ClassFactory.h"

#include "Utils/ComponentHelper.h"

#include "Editor/Editor.h"

#define DRAW_COMPONENT_SPAWNER(ComponentClass) \
	if (ImGui::MenuItem("Add "#ComponentClass)) \
	{	\
		NewComponent = ComponentHelper::CreateFreeComponent<ComponentClass>(#ComponentClass);\
		NewComponent->DefaultInitialise();\
	}

SpawnWindow::SpawnWindow()
	: m_WorldReference(World::Get())
{
	m_SpawnDistance = 20.0f;

	RegisterActorClass<StaticMeshActor>("Static Mesh Actor");
	RegisterActorClass<PointLightActor>("Point Light Actor");
	RegisterActorClass<SkyboxActor>("Skybox Actor");
	RegisterActorClass<CameraActor>("Camera Actor");
	RegisterActorClass<SpriteActor>("Sprite Actor");
}

void SpawnWindow::Update()
{

}

void SpawnWindow::Render()
{
	//TODO: Make centralised
	Editor::Settings& editorSettings = Editor::GetEditorSettings();
	if (editorSettings.m_DockFloatingWindows == true)
	{
		//Top Left
		Vector2 position = Editor::Get().GetSceneWindowPosition();
		ImGui::SetNextWindowPos(ImVec2(position.x + editorSettings.m_DockPadding.x, position.y + editorSettings.m_DockPadding.y));
	}

	if (ImGui::Begin("Actor Spawner"))
	{
		Actor* SpawnedActor = nullptr;

		//Initialise a cube mesh actor
		if (ImGui::Button("Cube"))
		{
			StaticMeshActor* MeshActor = m_WorldReference->SpawnActor<StaticMeshActor>("Cube");
			StaticMeshComponent* MeshComponent = MeshActor->GetMeshComponent();
			MeshComponent->SetMeshAndMaterial("Box", "Mat_FlatColour_White");
			SpawnedActor = MeshActor;
		}

		for (auto& ActorClass : m_ActorClassMap)
		{
			if (ImGui::Button(ActorClass.second.c_str()) && !SpawnedActor)
			{
				SpawnedActor = ClassFactory::Get().CreateObjectFromID<Actor>(ActorClass.first);
				World::Get()->RegisterActor(SpawnedActor);
			}
		}

		// Initialise the spawned actor if we spawned one
		if (SpawnedActor)
		{
			//TODO: Not working right
			Transform CameraTrans = RenderCommand::GetMainCamera()->GetCameraTransform();
			SpawnedActor->GetRootComponent()->SetWorldPosition(CameraTrans.m_Position +	(CameraTrans.m_Rotation.GetForwardVector() * m_SpawnDistance));

			switch (m_WorldReference->GetWorldState())
			{
			case WorldState::Editor: SpawnedActor->EditorBeginPlay(); break;
			case WorldState::Paused:
			case WorldState::InGame: SpawnedActor->BeginPlay(); break;
			}
		}
	}
	ImGui::End();
}

void SpawnWindow::DrawSpawnContextWindow(Actor* Parent)
{
	if (ImGui::BeginMenu("Add Component"))
	{
		WorldComponent* NewComponent = nullptr;

		DRAW_COMPONENT_SPAWNER(StaticMeshComponent);
		DRAW_COMPONENT_SPAWNER(SpriteComponent);
		DRAW_COMPONENT_SPAWNER(CameraComponent);
		DRAW_COMPONENT_SPAWNER(PointLightComponent);

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
		}

		ImGui::EndMenu();
	}
}
