#include "Flowpch.h"
#include "SelectionTool.h"
#include "Editor/Editor.h"
#include "Input/Input.h"

#include "Rendering\RenderCommand.h"

#include "Physics/Physics.h"

#include "GameFramework/Components/WorldComponent.h"
#include "GameFramework/Components/StaticMeshComponent.h"
#include "GameFramework/World.h"

#include "Editor/SelectionGizmo.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "GameFramework/Actor.h"

#include "Editor/UIComponents/Console.h"

SelectionTool::SelectionTool()
	: m_SelectedComponent(nullptr)
	, m_SpaceMode(ImGuizmo::WORLD)
{
	m_Gizmo = new SelectionGizmo();
	m_ConfigurationWindowOpen = true;
}

SelectionTool::~SelectionTool()
{
}

void SelectionTool::UpdateTool(float DeltaTime)
{
	m_Gizmo->UpdateSelection();

	for (auto& line : m_PreviousLines)
	{
		World::GetLineBatcher_S().AddLine(line.first, line.second, m_DebugLineColor);
	}
}

void SelectionTool::RenderTool()
{
	m_Gizmo->Render();
}

void SelectionTool::BeginPlay()
{
	m_Gizmo->InitialisePhysics();
}

void SelectionTool::RenderImGuiGizmo()
{
	//TODO: Do this properly instead of lazy
	//TODO: Try cherno's compose/decompose to avoid the issues
	if (m_SelectedComponent != nullptr)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(); //TODO: Put it in imgui

		Editor& editor = Editor::Get();
		Vector2 rectPos = editor.GetSceneWindowPosition();
		Vector2 rectSize = editor.GetSceneWindowSize();
		ImGuizmo::SetRect(rectPos.x, rectPos.y, rectSize.x, rectSize.y);

		Rotator inRotation = m_SelectedComponent->GetWorldRotation();
	
		//TODO: store a matrix whilst transforming the object?
		DirectX::XMFLOAT4X4 matrix;
		DirectX::XMStoreFloat4x4(&matrix, reinterpret_cast<RenderableComponent*>(m_SelectedComponent)->GetTransformXM());

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
			reinterpret_cast<RenderableComponent*>(m_SelectedComponent)->SetMatrix(m_CurrentMatrix);

			float position[3];
			float rotation[3];
			float scale[3];

			ImGuizmo::DecomposeMatrixToComponents((float*)&m_CurrentMatrix, position, rotation, scale); //TODO: THe matrix is fine, but the decomposed rotation isnt?

			m_SelectedComponent->SetWorldPosition(*(Vector3*)&position);
			m_SelectedComponent->SetWorldRotation(*(Rotator*)&rotation);
			m_SelectedComponent->SetWorldScale(*(Vector3*)&scale);
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

			//Check if we hit a selection gimzo
			if (SelectionGizmo* Gizmo = dynamic_cast<SelectionGizmo*>(actor))
			{
				Axis Axis = SelectionGizmo::TagToAxis(comp->m_Tag);
				if (Axis == Axis::None)
				{
					FLOW_ENGINE_ERROR("Inspector::OnMouseClicked: Selected Selection Gizmo but failed to identify mesh");
					return true;
				}

				Gizmo->OnSelected(Axis, m_SelectedComponent);
				return true;
			}

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
		m_Gizmo->OnNewComponentSelected(nullptr);

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
		m_Gizmo->SetTransformationMode(SelectionGizmo::Translation);
	}

	if (e.GetKeyCode() == KEY_E)
	{
		m_transformationMode = TransformMode::Rotate;
		m_Gizmo->SetTransformationMode(SelectionGizmo::Rotation);
	}

	if (e.GetKeyCode() == KEY_R)
	{
		m_transformationMode = TransformMode::Scale;
		m_Gizmo->SetTransformationMode(SelectionGizmo::Scale);
	}

	return false;
}

bool SelectionTool::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
{
	if (e.GetMouseButton() == MOUSE_LEFT)
	{
		if (m_Gizmo->GetSelectedAxis() != Axis::None)
		{
			m_Gizmo->OnDeselected();
		}
	}

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
		DirectX::XMStoreFloat4x4(&m_CurrentMatrix, reinterpret_cast<RenderableComponent*>(m_SelectedComponent)->GetTransformXM());//TODO:

		if (m_SelectedComponent)
		{
			m_SelectedComponent->OnViewportSelected();
		}
	}

	//If we didnt hit anything, reset the gizmo
	if (!m_SelectedActor)
	{
		if (m_SelectedComponent)
		{
			m_SelectedComponent->OnViewportDeselected();
			m_SelectedComponent = nullptr;
		}

		m_Gizmo->OnDeselected();
		m_Gizmo->OnNewComponentSelected(nullptr);

		//Hide Gizmo
		if (m_Gizmo->IsVisible())
		{
			m_Gizmo->RemoveCollidersFromWorld(World::Get());
			m_Gizmo->SetVisibility(false);
		}
	}
	//Otherwise add it to the world
	else
	{
		//Show Gizmo
		if (!m_Gizmo->IsVisible())
		{
			m_Gizmo->AddCollidersToWorld(World::Get());
			m_Gizmo->SetVisibility(true);
		}

		m_Gizmo->OnNewComponentSelected(NewComponent);
		m_Gizmo->UpdatePosition(Parent->GetLocation());
		m_Gizmo->SetScale(Vector3(1.5f, 1.5f, 1.5f));
	}
}

ImGuizmo::OPERATION SelectionTool::TranslateTransformation(TransformMode mode)
{
	switch (mode)
	{
	case TransformMode::Translate:
		return ImGuizmo::OPERATION::TRANSLATE;
	case TransformMode::Rotate:
		return ImGuizmo::OPERATION::ROTATE;
	case TransformMode::Scale:
		return ImGuizmo::OPERATION::SCALE;
	default:
		return ImGuizmo::OPERATION::TRANSLATE;
	}
}
