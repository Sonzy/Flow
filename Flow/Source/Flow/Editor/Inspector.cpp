#include "Flowpch.h"
#include "Inspector.h"
#include "ThirdParty\ImGui\imgui.h"
#include "ThirdParty\ImGui\misc\cpp\imgui_stdlib.h"
#include "Flow\GameFramework\World.h"

#include "Flow\GameFramework\Actor.h"
#include "Flow\Editor\SelectionGizmo.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Input\Input.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"
#include "btBulletDynamicsCommon.h"

#include "Flow\GameFramework\Components\WorldComponent.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"
#include "Flow\GameFramework\Components\CameraComponent.h"

#include "Flow/Events/KeyEvent.h"

#include "Flow/Editor/EditorLayer.h"


Inspector::Inspector()
	: _CurrentWorld(nullptr)
	, m_FocusedComponent(nullptr)
{
}

void Inspector::BeginPlay()
{

}

void Inspector::RenderInspector()
{
	if (ImGui::Begin("Inspector"))
	{
		if (!m_FocusedComponent)
		{
			ImGui::End();
			return;
		}

		Actor* ParentActor = m_FocusedComponent->GetParentActor();

		ImGui::InputText("ObjectName", &ParentActor->GetWritableName());
		ImGui::TextColored(IMGUI_ORANGE, (std::string("Selected Component: ") + (m_FocusedComponent ? m_FocusedComponent->GetName() : std::string("None"))).c_str());

		ImGui::Separator();

		if (!_HideTree && ParentActor->GetRootComponent())
			ParentActor->GetRootComponent()->DrawInspectionTree(m_FocusedComponent);

		if (ImGui::Button(_HideTree ? "Show Tree" : "Collapse Tree"))
		{
			_HideTree = !_HideTree;
		}

		ImGui::Separator(); //==========================================


		if (m_FocusedComponent)
		{
			ImGui::TextColored(IMGUI_YELLOW, "Component Transform");
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			DrawSelectedComponentTransform();
		}

		ImGui::Separator();

		//_FocusedItem->DrawDetailsWindow(_FocusedItemChanged); //TODO: update focused item changed
		ParentActor->DrawDetailsWindow(false);

		ImGui::Separator();

		if (m_FocusedComponent)
		{
			//TODO: Not sure what the difference is
			m_FocusedComponent->DrawComponentDetailsWindow(); //TODO: Move this where I want
			m_FocusedComponent->DrawDetailsWindow(false);
		}
	}
	ImGui::End();
}

void Inspector::RenderHeirarchy()
{
	if (ImGui::Begin("Scene Heirarchy"))
	{
		ImGui::Text(std::string("Level: " + _CurrentWorld->GetName()).c_str());

		ImGui::Separator();

		for (auto Object : _CurrentWorld->GetActors())
		{
			bool NodeOpen = ImGui::TreeNode(Object->GetName().c_str());

			if (ImGui::IsItemClicked())
			{
				//TODO:
				SetFocus(Object.get()->GetRootComponent());

				//_FocusedItem = Object.get();
				//_FocusedItemChanged = true;
				//_FocusedComponent = _FocusedItem->GetRootComponent();

				//TODO: Make select obj function
						//TODO: implement in toool
				//if (!_Selector->IsVisible())
				//{
				//	_Selector->AddCollidersToWorld(World::Get());
				//	_Selector->SetVisibility(true);
				//}
				//_Selector->OnNewComponentSelected(Object->GetRootComponent());
				//_Selector->UpdatePosition(Object->GetLocation());
			}

			if(NodeOpen)
				ImGui::TreePop();
		}
	}
	ImGui::End();
}

void Inspector::SetCurrentWorld(World* WorldReference)
{
	_CurrentWorld = WorldReference;
}

bool Inspector::OnMouseClicked(MouseButtonPressedEvent& e)
{
	return false;
}

bool Inspector::OnMouseReleased(MouseButtonReleasedEvent& e)
{
	//TODO: implement in tool
	//if (_Selector->GetSelectedAxis() != SelectedAxis::None)
	//{
	//	_Selector->OnDeselected();
	//	return true;
	//}

	return false;
}

bool Inspector::OnKeyPressed(KeyPressedEvent& e)
{
	//if (e.GetKeyCode() == FLOW_KEY_DELETE && _FocusedItem)
	//{
	//	//std::shared_ptr<Actor> ActorPtr(_FocusedItem);
	//	//World::Get()->DestroyActor(ActorPtr);
	//
	//	World::Get()->DestroyActor(_FocusedItem);
	//
	//	//_FocusedItem = nullptr;
	//
	//	if (_FocusedComponent)
	//		_FocusedComponent->OnViewportDeselected();
	//	_FocusedComponent = nullptr;
	//
	//	//TODO: implement in toool
	//	//_Selector->SetScale(Vector3(1.0f, 1.0f, 1.0f));
	//	//_Selector->OnDeselected();
	//	//_Selector->OnNewComponentSelected(nullptr);
	//	//
	//	//if (_Selector->IsVisible())
	//	//{
	//	//	_Selector->RemoveCollidersFromWorld(World::Get());
	//	//	_Selector->SetVisibility(false);
	//	//}
	//	return true;
	//}

	return false;
}

void Inspector::UpdateSelectedComponent(WorldComponent* NewComp)
{
	//EditorLayer::GetEditor()->GetInspector()->_FocusedComponent = NewComp;
}

WorldComponent* Inspector::GetSelectedComponent()
{
	//return EditorLayer::GetEditor()->GetInspector()->_FocusedComponent;
	return nullptr;
}

void Inspector::Update()
{
	//_Selector->UpdateSelection();

	//_FocusedItemChanged = false;
}

void Inspector::Render()
{
	RenderInspector();
	RenderHeirarchy();
}

void Inspector::DrawSelectedComponentTransform()
{
	bool bUpdate = false;
	bUpdate |= ImGui::InputFloat3("Position", (float*)m_FocusedComponent->GetWriteablePosition(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
	bUpdate |= ImGui::InputFloat3("Rotation", (float*)m_FocusedComponent->GetWriteableRotation(), 1, ImGuiInputTextFlags_EnterReturnsTrue);
	if (ImGui::InputFloat3("Scale", (float*)m_FocusedComponent->GetWriteableScale(), 1, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		bUpdate = true;
		m_FocusedComponent->UpdateCollisionScale();
	}
	//TODO:
	//if (bUpdate)
	//	_FocusedComponent->UpdatePhysicsBody(true);
	//TODO: Update collision scale
}

//SelectionGizmo* Inspector::GetSelector() const
//{
//	return _Selector;
//}