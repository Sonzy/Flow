#define NOMINMAX
#include "Flowpch.h"
#include "SelectionTool.h"
#include "Flow/Editor/Editor.h"
#include "Flow/Input/Input.h"

#include "Flow\Rendering\RenderCommand.h"

#include "Flow/Physics/Physics.h"

#include "Flow/GameFramework/Components/WorldComponent.h"
#include "Flow/GameFramework/Components/StaticMeshComponent.h"
#include "Flow/GameFramework/World.h"

#include "Flow/Editor/SelectionGizmo.h"
#include "Flow/Events/KeyEvent.h"
#include "Flow/Events/MouseEvent.h"

#include "Flow/GameFramework/Actor.h"

#include "Flow/Editor/Inspector.h"

SelectionTool::SelectionTool()
	: m_SelectedComponent(nullptr)
{
	m_Gizmo = new SelectionGizmo();
	m_InspectorWindow = Editor::GetEditor()->GetInspector();
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

bool SelectionTool::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() != MOUSE_LEFT)
		return false;

	//Calculate the ray bounds
	IntVector2 MousePosition = Input::GetMousePosition();
	Vector3 Start = RenderCommand::GetMainCamera()->GetCameraPosition();
	Vector3 Direction = RenderCommand::GetScreenToWorldDirectionVector( 	//TODO: normalise direction
		MousePosition.x,
		MousePosition.y,
		Editor::GetEditor()->GetSceneWindowSize(),
		Editor::GetEditor()->GetSceneWindowPosition());
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
	bool HitAnything = result.hasHit();
	if (HitAnything)
	{
		//TODO: Move this check to check the object that last drew the pixel
		//Check if we hit a selection gimzo
		for (int i = 0; i < result.m_collisionObjects.size(); i++)
		{
			WorldComponent* comp = reinterpret_cast<WorldComponent*>(result.m_collisionObjects[i]->getUserPointer());
			Actor* actor = comp->GetParentActor();

			if (SelectionGizmo* Gizmo = dynamic_cast<SelectionGizmo*>(actor))
			{
				Axis Axis = Axis::None;

				if (comp->m_Tag._Equal("ArrowX"))
					Axis = Axis::X;
				else if (comp->m_Tag._Equal("ArrowY"))
					Axis = Axis::Y;
				else if (comp->m_Tag._Equal("ArrowZ"))
					Axis = Axis::Z;

				if (Axis == Axis::None)
				{
					FLOW_ENGINE_ERROR("Inspector::OnMouseClicked: Selected Selection Gizmo but failed to identify mesh");
					return true;
				}

				Gizmo->OnSelected(Axis, m_SelectedComponent);
				return true;
			}
		}

		HitComponent = reinterpret_cast<WorldComponent*>(result.m_collisionObject->getUserPointer());
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
		if (m_SelectedActor)
		{
			m_SelectedActor->OnViewportDeselected();
			World::Get()->DestroyActor(m_SelectedActor);
			m_SelectedActor = nullptr;
		}

		//TODO: implement in toool
		//_Selector->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		//_Selector->OnDeselected();
		//_Selector->OnNewComponentSelected(nullptr);
		//
		//if (_Selector->IsVisible())
		//{
		//	_Selector->RemoveCollidersFromWorld(World::Get());
		//	_Selector->SetVisibility(false);
		//}

		return true;
	}

	if (e.GetKeyCode() == KEY_W)
	{
		m_Gizmo->SetTransformationMode(SelectionGizmo::Translation);
	}

	if (e.GetKeyCode() == KEY_E)
	{
		m_Gizmo->SetTransformationMode(SelectionGizmo::Rotation);
	}

	if (e.GetKeyCode() == KEY_R)
	{
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
			m_SelectedComponent->OnViewportSelected();
		}

		m_InspectorWindow->SetFocus(m_SelectedComponent);
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
