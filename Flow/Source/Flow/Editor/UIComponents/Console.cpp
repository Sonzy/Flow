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
	switch (Type)
	{
	case Console::Log:		return Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	case Console::Warning:	return Vector4(1.0f, 1.0f, 0.0f, 1.0f);
	case Console::Error:	return Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	default:				return Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	}
}

// Console::Message Definitions //////////////////////////////////////////////////

Console::Message::Message(MessageType Type, const char* NewMessage)
	: m_Type(Type)
	, m_Message(NewMessage)
{
}
