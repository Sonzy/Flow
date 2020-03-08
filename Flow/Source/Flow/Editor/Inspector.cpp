#include "Flowpch.h"
#include "Inspector.h"
#include "ThirdParty\ImGui\imgui.h"
#include "Flow\GameFramework\World.h"
#include "Flow\GameFramework\WorldObject.h"

namespace Flow
{
	Inspector::Inspector()
		: m_FocusedItem(nullptr)
	{
	}

	void Inspector::RenderInspector()
	{
		if (ImGui::Begin("Inspector"))
		{
			ImGui::Text("Selected Item:");
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
		FLOW_ENGINE_LOG("OnMouseClicked: TODO: collision with objects in scene");
		return false;
	}
}