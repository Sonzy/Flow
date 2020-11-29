#include "Flowpch.h"
#include "Console.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow/Editor/Editor.h"

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
	return Editor::Get().GetConsole();
}

Console* Console::GetSafe()
{
	Editor& Editor = Editor::Get();

	if (Editor.IsInitialised() == false)
	{
		return nullptr;
	}

	return &Editor.GetConsole();
}

void Console::LogMessage(const Console::Message& newMessage)
{
	if (Console* cons = Console::GetSafe())
	{
		cons->m_MessageHistory.push_back(newMessage);
	}
}
