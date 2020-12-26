#include "Flowpch.h"
#include "Console.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow/Editor/Editor.h"

Console::Console()
{
	Log::InitConsole(this);
}

Console::~Console()
{
}

void Console::Update()
{
}

void Console::Render()
{
	if (ImGui::Begin("Output"))
	{
		if (ImGui::BeginChild("Console", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar))
		{
			for (auto& msg : m_MessageHistory)
			{
				ImGui::TextColored(TypeToColor(msg.m_Type), msg.m_Message.c_str());
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

Console& Console::Get()
{
	return *Editor::Get().GetUIComponent<Console>();
}

Console* Console::GetSafe()
{
	return Editor::Get().GetUIComponent<Console>();
}

void Console::PushMessage(MessageType type, const char* Message)
{
	m_MessageHistory.emplace_back(type, Message);
}

Vector4 Console::TypeToColor(MessageType Type)
{
	Editor::Settings& settings = Editor::GetSettings();

	switch (Type)
	{
	case Console::Log:		return settings.m_consoleLogColor;
	case Console::Warning:	return settings.m_consoleWarningColor;
	case Console::Error:	return settings.m_consoleErrorColor;
	default:				return Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	}
}

// Console::Message Definitions //////////////////////////////////////////////////

Console::Message::Message(MessageType Type, const char* NewMessage)
	: m_Type(Type)
	, m_Message(NewMessage)
{
}
