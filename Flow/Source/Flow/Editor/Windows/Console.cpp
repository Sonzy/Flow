#include "Flowpch.h"
#include "Console.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow/Editor/EditorLayer.h"

Console::Console()
{
}

Console::~Console()
{
}

void Console::Draw()
{
	if (ImGui::Begin("Output"))
	{
		if (ImGui::BeginChild("Console", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar))
		{
			for (auto& msg : m_MessageHistory)
			{
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), msg.m_Message);
			}
		}
		ImGui::EndChild();
		ImGui::End();
	}
}

Console& Console::Get()
{
	return EditorLayer::GetEditor()->GetConsole();
}

Console* Console::GetSafe()
{
	if (EditorLayer* Editor = EditorLayer::GetEditor())
	{
		if (!Editor->Initialised)
		{
			return nullptr;
		}

		return &Editor->GetConsole();
	}

	return nullptr;
}

void Console::LogMessage(const Console::Message& newMessage)
{
	if (Console* cons = Console::GetSafe())
	{
		cons->m_MessageHistory.push_back(newMessage);
	}
}
