#pragma once
#include "Maths/Vector4.h"
#include "UIComponent.h"

// Class Definition /////////////////////////////////////////////

class Console : public UIComponent
{
public:

	// Public Enum Definitions ////////////////////////////////////////////////////

	enum MessageType
	{
		Log,
		Warning,
		Error
	};

public:
	// Public Struct Definitions //////////////////////////////////////////////////

	struct Message
	{
		Message(MessageType Type, const char* NewMessage);

		MessageType	m_Type;
		std::string m_Message;
	};

public:

	// Public Function Definitions ////////////////////////////////////////////////

										Console();
										~Console();

	void								Update() override;
	void								Render() override;

	static Console&						Get();
	static Console*						GetSafe();

	void								PushMessage(MessageType type, const char* Message);

private:

	// Private Functions //////////////////////////////////////////////////////////

	Vector4								TypeToColor(MessageType Type);

private:

	std::vector<Console::Message> m_MessageHistory;
};