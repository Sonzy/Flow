#include "Flowpch.h"
#include "SpawnWindow.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow/GameFramework/World.h"

#include "Flow/GameFramework/Actors/StaticMeshActor.h"
#include "Flow/GameFramework/Actors/PointLightActor.h"
#include "Flow/GameFramework/Actors/SkyboxActor.h"
#include "Flow/GameFramework/Actors/CameraActor.h"

#include "Flow/GameFramework/Components/StaticMeshComponent.h"

#include "Flow/GameFramework/Other/ClassFactory.h"

SpawnWindow::SpawnWindow(World* WorldReference)
	: _WorldReference(WorldReference)
{
	_SpawnDistance = 20.0f;

	RegisterActorClass<StaticMeshActor>("Static Mesh Actor");
	RegisterActorClass<PointLightActor>("Point Light Actor");
	RegisterActorClass<SkyboxActor>("Skybox Actor");
	RegisterActorClass<CameraActor>("Camera Actor");
}

void SpawnWindow::Draw()
{
	if (ImGui::Begin("Actor Spawner"))
	{
		std::shared_ptr<Actor> SpawnedActor;

		//Initialise a cube mesh actor
		if (ImGui::Button("Cube"))
		{
			auto MeshActor = _WorldReference->SpawnActor<StaticMeshActor>("Cube");
			auto MeshComponent = MeshActor->GetMeshComponent();
			MeshComponent->SetMeshAndMaterial("Box", "Mat_FlatColour_White");
			SpawnedActor = std::dynamic_pointer_cast<Actor>(MeshActor);
		}

		for (auto& ActorClass : _ActorClassMap)
		{
			if (ImGui::Button(ActorClass.second.c_str()) && !SpawnedActor)
			{
				SpawnedActor = std::shared_ptr<Actor>(ClassFactory::Get().CreateObjectFromID<Actor>(ActorClass.first));
				World::Get()->AddDefaultInitialisedActor(SpawnedActor);
			}
		}

		// Initialise the spawned actor if we spawned one
		if (SpawnedActor)
		{
			Transform CameraTrans = RenderCommand::GetMainCamera()->GetCameraTransform();
			Vector ForwardVector = CameraTrans._Rotation.ToVector().Normalize();
			SpawnedActor->GetRootComponent()->SetWorldPosition(CameraTrans._Position +
				(ForwardVector * _SpawnDistance));

			switch (_WorldReference->GetWorldState())
			{
			case WorldState::Editor: SpawnedActor->EditorBeginPlay(); break;
			case WorldState::Paused:
			case WorldState::InGame: SpawnedActor->BeginPlay(); break;
			}
		}
	}
	ImGui::End();
}