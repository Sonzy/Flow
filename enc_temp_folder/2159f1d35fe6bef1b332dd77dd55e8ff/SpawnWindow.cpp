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
	: m_WorldReference(WorldReference)
{
	m_SpawnDistance = 20.0f;

	RegisterActorClass<StaticMeshActor>("Static Mesh Actor");
	RegisterActorClass<PointLightActor>("Point Light Actor");
	RegisterActorClass<SkyboxActor>("Skybox Actor");
	RegisterActorClass<CameraActor>("Camera Actor");
}

void SpawnWindow::Draw()
{
	if (ImGui::Begin("Actor Spawner"))
	{
		Actor* SpawnedActor = nullptr;

		//Initialise a cube mesh actor
		if (ImGui::Button("Cube"))
		{
			auto MeshActor = m_WorldReference->SpawnActor<StaticMeshActor>("Cube");
			auto MeshComponent = MeshActor->GetMeshComponent();
			MeshComponent->SetMeshAndMaterial("Box", "Mat_FlatColour_White");
			SpawnedActor = MeshActor;
		}

		for (auto& ActorClass : m_ActorClassMap)
		{
			if (ImGui::Button(ActorClass.second.c_str()) && !SpawnedActor)
			{
				SpawnedActor = ClassFactory::Get().CreateObjectFromID<Actor>(ActorClass.first);
				World::Get()->AddDefaultInitialisedActor(SpawnedActor);
			}
		}

		// Initialise the spawned actor if we spawned one
		if (SpawnedActor)
		{
			//Transform CameraTrans = RenderCommand::GetMainCamera()->GetCameraTransform();
			//Vector3 ForwardVector = static_cast<Vector3>(CameraTrans.m_Rotation).Normalize();

			//TODO: Not working right
			Transform CameraTrans = RenderCommand::GetMainCamera()->GetCameraTransform();
			Rotator CameraRot = CameraTrans.m_Rotation;
			const DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			const auto lookVector = DirectX::XMVector3Transform(Forward, DirectX::XMMatrixRotationRollPitchYaw(CameraRot.Pitch, CameraRot.Yaw, CameraRot.Roll));
			Vector3 forward;
			DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&forward), lookVector);

			SpawnedActor->GetRootComponent()->SetWorldPosition(CameraTrans.m_Position +	(forward * m_SpawnDistance));

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