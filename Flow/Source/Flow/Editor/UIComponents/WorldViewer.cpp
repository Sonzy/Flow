#include "pch.h"

#if WITH_EDITOR

#include "WorldViewer.h"
#include "GameFramework/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Components/Component.h"

WorldViewer::WorldViewer()
	: UIComponent()
	, m_World(World::Get())
{
	m_windowVisible = true;
}

void WorldViewer::Update()
{
}

void WorldViewer::Render()
{
	if (m_windowVisible)
	{
		if (ImGui::Begin("World Viewer"))
		{
			if (ImGui::CollapsingHeader("Actor List"))
			{
				const std::unordered_map<FGUID, Actor*> actorList = m_World.GetActorMap();
				for (auto data : actorList)
				{
					ImGui::Text("%lu - %s", data.first, data.second->GetName().c_str());
				}
			}

			if (ImGui::CollapsingHeader("Component List"))
			{
				const std::unordered_map<FGUID, Component*> componentList = m_World.GetComponentMap();
				for (auto data : componentList)
				{
					ImGui::Text("%lu - %s", data.first, data.second->GetName().c_str());
				}
			}


		}
		ImGui::End();
	}
}

#endif //WITH_EDITOR
