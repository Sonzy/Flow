#include "Flowpch.h"
#include "Inspector.h"
#include "ThirdParty\ImGui\imgui.h"
#include "Flow\GameFramework\World.h"
#include "Flow\GameFramework\WorldObject.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Input\Input.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"
#include "btBulletDynamicsCommon.h"

#include "Flow\GameFramework\Components\WorldComponent.h"

#include "Flow\Editor\SelectionGizmo.h"


namespace Flow
{
	Inspector::Inspector(SelectionGizmo* Selector)
		: m_CurrentWorld(nullptr), m_FocusedItem(nullptr), m_Selector(Selector)
	{
	}

	void Inspector::RenderInspector()
	{
		if (ImGui::Begin("Inspector"))
		{
			if (m_FocusedItem)
				m_FocusedItem->DrawDetailsWindow(ObjChanged);
		}
		ImGui::End();
	}

	void Inspector::RenderHeirarchy()
	{
		if (ImGui::Begin("Heirarchy"))
		{
			ImGui::Text(std::string("Level: " + m_CurrentWorld->GetName()).c_str());

			ImGui::Separator();

			for (auto Object : m_CurrentWorld->m_WorldObjects)
			{
				ImGui::Text(Object->GetName().c_str());
			}
		}
		ImGui::End();
	}

	void Inspector::SetCurrentWorld(World* WorldReference)
	{
		m_CurrentWorld = WorldReference;
	}

	bool Inspector::OnMouseClicked(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() != FLOW_MOUSE_LEFT)
			return false;

		DirectX::XMFLOAT3 Pos = RenderCommand::GetCamera().GetPosition();
		IntVector2D MousePosition = Input::GetMousePosition();
		Vector Start = Vector(Pos.x, Pos.y, Pos.z);
		Vector Direction = RenderCommand::GetScreenToWorldDirectionVector(MousePosition.X, MousePosition.Y);
		Vector End = Start + (Direction * 1000.0f);

		btCollisionWorld::ClosestRayResultCallback Ray = World::WorldTrace(Start, End);

		WorldObject* HitObject = Ray.hasHit() ? static_cast<WorldComponent*>(Ray.m_collisionObject->getUserPointer())->GetParentWorldObject() : nullptr;
		std::string HitObjectName = HitObject ? HitObject->GetName() : "Nothing";

		m_FocusedItem = HitObject;

		//Vector HitLoc = Vector(Ray.m_hitPointWorld.x(), Ray.m_hitPointWorld.y(), Ray.m_hitPointWorld.z());
		//	FLOW_ENGINE_LOG("Inspector::OnMouseClicked: Start: {0}, End: {1}, Hit: {2}, HitLoc: {3}", Start, End, HitObjectName, HitLoc);
		if (Ray.hasHit() && static_cast<SelectionGizmo*>(Ray.m_collisionObject->getUserPointer()))
		{
			FLOW_ENGINE_LOG("We hit the selection. TODO: Detection on each arro");
		}

		if (!m_FocusedItem)
		{
			m_Selector->SetVisibility(false);
			m_Selector->SetScale(Vector(1.0f, 1.0f, 1.0f));
		}
		else
		{
			//m_Selector->UpdatePosition(HitObject->GetLocation());
			//m_Selector->SetScale(HitObject->GetScale().X);

			//TODO: Re-enable when setting this back up
			//m_Selector->SetVisibility(true);
		}

		ObjChanged = true;


		return true;
	}

	void Inspector::Update()
	{
		RenderInspector();
		RenderHeirarchy();

		ObjChanged = false;
	}

	SelectionGizmo* Inspector::GetSelector()
	{
		return m_Selector;
	}
}