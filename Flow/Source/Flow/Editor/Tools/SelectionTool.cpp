#include "Flowpch.h"
#include "SelectionTool.h"
#include "Editor/Editor.h"
#include "Input/Input.h"

#include "Rendering\RenderCommand.h"

#include "Physics/Physics.h"

#include "GameFramework/Components/WorldComponent.h"
#include "GameFramework/Components/StaticMeshComponent.h"
#include "GameFramework/World.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "GameFramework/Actor.h"

#include "Editor/UIComponents/Console.h"

SelectionTool::SelectionTool()
	: m_SelectedComponent(nullptr)
	, m_SpaceMode(ImGuizmo::WORLD)
{

}

SelectionTool::~SelectionTool()
{
}

void SelectionTool::UpdateTool(float DeltaTime)
{
	for (auto& line : m_PreviousLines)
	{
		World::GetLineBatcher_S().AddLine(line.first, line.second, m_DebugLineColor);
	}
}

void SelectionTool::RenderTool()
{
}

void SelectionTool::BeginPlay()
{
}

void SelectionTool::RenderImGuiGizmo()
{
	if (m_SelectedComponent != nullptr)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(); //TODO: Put this inside ImGui?

		Editor& editor = Editor::Get();
		Vector2 rectPos = editor.GetSceneWindowPosition();
		Vector2 rectSize = editor.GetSceneWindowSize();
		ImGuizmo::SetRect(rectPos.x, rectPos.y, rectSize.x, rectSize.y);
	
		//= Convert view matrix to valid format

		DirectX::XMFLOAT4X4 fViewMatrix;
		DirectX::XMStoreFloat4x4(&fViewMatrix, RenderCommand::GetMainCamera()->GetViewMatrix());

		DirectX::XMFLOAT4X4 fProjectionMatrix;
		DirectX::XMStoreFloat4x4(&fProjectionMatrix, RenderCommand::GetMainCamera()->GetProjectionMatrix());

		ImGuizmo::Manipulate(
			reinterpret_cast<float*>(&fViewMatrix),
			reinterpret_cast<float*>(&fProjectionMatrix),
			TranslateTransformation(m_transformationMode),
			m_SpaceMode,
			reinterpret_cast<float*>(&m_CurrentMatrix)
		);

		if (ImGuizmo::IsUsing())
		{			
			DirectX::XMVECTOR quatRotation;
			DirectX::XMVECTOR position;
			DirectX::XMVECTOR scale;
			DirectX::XMMatrixDecompose(&scale, &quatRotation, &position, DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&m_CurrentMatrix)));

			DirectX::XMFLOAT3 pos, sca;
			DirectX::XMStoreFloat3(&pos, position);
			DirectX::XMStoreFloat3(&sca, scale);

			m_SelectedComponent->SetWorldPosition(pos);
			m_SelectedComponent->SetWorldRotation(Maths::QuaternionToEulers(quatRotation));
			m_SelectedComponent->SetWorldScale(sca);
		}
	}
}

bool SelectionTool::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() != MOUSE_LEFT)
		return false;

	if (ImGuizmo::IsOver())
	{
		return true;
	}

	//TODO: Use a picking depth buffer

	//Calculate the ray bounds
	IntVector2 MousePosition = Input::GetMousePosition();
	Vector3 Start = RenderCommand::GetMainCamera()->GetCameraPosition();
	Vector3 Direction = RenderCommand::GetScreenToWorldDirectionVector( 	//TODO: normalise direction
		MousePosition.x,
		MousePosition.y,
		Editor::Get().GetSceneWindowSize(),
		Editor::Get().GetSceneWindowPosition());
	Vector3 End = Start + (Direction * 1000.0f);

	if (m_DrawSelectionLines)
	{
		m_PreviousLines.emplace_back(Start, End); //TODO: Do properly
	}

	//TODO: Filter out other objects that arent world components
	//Raytrace into the world 

	Physics::TraceResultMulti result = Physics::RayTraceMulti(Start, End);

	//Store the hit objects
	WorldComponent* HitComponent = nullptr;
	Actor* HitActor = nullptr;

	if (result.hasHit())
	{
		float ClosestDistance = 99999999.0f;
		WorldComponent* FoundComponent = nullptr;
		Actor* FoundActor = nullptr; //Only check for a gizmo if this is true

		//TODO: Move this check to check the object that last drew the pixel
		for (int i = 0; i < result.m_collisionObjects.size(); i++)
		{
			WorldComponent* comp = reinterpret_cast<WorldComponent*>(result.m_collisionObjects[i]->getUserPointer());
			Actor* actor = comp->GetParentActor();

			//Store the closest found one.
			float DistanceSquared = Maths::DistanceSquared(Start, comp->GetWorldPosition());
			if (DistanceSquared < ClosestDistance)
			{
				ClosestDistance = DistanceSquared;
				FoundComponent = comp;
				FoundActor = actor;
			}
		}

		HitComponent = FoundComponent;
		HitActor = HitComponent->GetParentActor();
	}

	SelectComponent(HitComponent);

	return false;
}

bool SelectionTool::OnKeyPressed(KeyPressedEvent& e)
{
	if (e.GetKeyCode() == KEY_DELETE && m_SelectedActor)
	{
		//Deselect and delete component
		if (m_SelectedComponent)
		{
			m_SelectedComponent->OnViewportDeselected();
			m_SelectedComponent = nullptr;
		}

		//Deselect and delete actor
		if (m_SelectedActor != nullptr)
		{
			m_SelectedActor->OnViewportDeselected();
			World::Get()->DestroyActor(m_SelectedActor->GetGuid());
			m_SelectedActor = nullptr;
		}

		return true;
	}

	if (e.GetKeyCode() == KEY_W)
	{
		m_transformationMode = TransformMode::Translate;
	}

	if (e.GetKeyCode() == KEY_E)
	{
		m_transformationMode = TransformMode::Rotate;
	}

	if (e.GetKeyCode() == KEY_R)
	{
		m_transformationMode = TransformMode::Scale;
	}

	return false;
}

bool SelectionTool::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
{
	return false;
}

void SelectionTool::DrawConfigWindow()
{
	if (ImGui::Begin("Selection Tool Config", &m_ConfigurationWindowOpen))
	{
		//Clear the stored lines on setting change
		if (ImGui::Checkbox("Draw debug lines in world from click", &m_DrawSelectionLines))
		{
			m_PreviousLines.clear();
		}

		if (ImGui::Button(m_SpaceMode == ImGuizmo::WORLD ? "World" : "Local"))
		{
			m_SpaceMode = (m_SpaceMode == ImGuizmo::WORLD) ? ImGuizmo::LOCAL : ImGuizmo::WORLD;
		}

		ImGui::ColorEdit3("Debug Line Color", reinterpret_cast<float*>(&m_DebugLineColor));//, ImGuiColorEditFlags_NoInputs);		
	}
	ImGui::End();
}

void SelectionTool::SelectComponent(WorldComponent* NewComponent)
{
	Actor* Parent = NewComponent != nullptr ? NewComponent->GetParentActor() : nullptr;

	//Pass on the onclicked event
	if (m_SelectedActor != Parent)
	{
		if (m_SelectedActor)
		{
			m_SelectedActor->OnViewportDeselected();
		}

		m_SelectedActor = Parent;

		if (m_SelectedActor)
		{
			m_SelectedActor->OnViewportSelected();
		}
	}

	//Pass on the on clicked event
	if (m_SelectedComponent != NewComponent)
	{
		if (m_SelectedComponent)
		{
			m_SelectedComponent->OnViewportDeselected();
		}

		m_SelectedComponent = NewComponent;

		if (m_SelectedComponent)
		{
			m_CurrentMatrix = m_SelectedComponent->GetTransformationMatrix();
			m_SelectedComponent->OnViewportSelected();
		}
	}

	//If we didnt hit anything, reset the gizmo
	if (m_SelectedActor == nullptr)
	{
		if (m_SelectedComponent)
		{
			m_SelectedComponent->OnViewportDeselected();
			m_SelectedComponent = nullptr;
		}
	}
}

ImGuizmo::OPERATION SelectionTool::TranslateTransformation(TransformMode mode)
{
	switch (mode)
	{
	case TransformMode::Translate:	return ImGuizmo::OPERATION::TRANSLATE;
	case TransformMode::Rotate:		return ImGuizmo::OPERATION::ROTATE;
	case TransformMode::Scale:		return ImGuizmo::OPERATION::SCALE;
	default:						return ImGuizmo::OPERATION::TRANSLATE;
	}
}
