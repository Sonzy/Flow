#pragma once

class Console
{
public:

	enum MessageType
	{
		Log,
		Warning,
		Error
	};

	struct Message
	{
		Message(MessageType type, const char* format, ...)
			: m_Type(type)
		{
			va_list _ArgList;

			__crt_va_start(_ArgList, format);
			sprintf_s(m_Message, 256, format, _ArgList);
			__crt_va_end(_ArgList);
		}

		MessageType m_Type;
		char m_Message[256];
	};

public:
	Console();
	~Console();

	void Draw();

	static Console& Get();
	static Console* GetSafe();

	static void LogMessage(const Console::Message& newMessage);

private:

	std::vector<Console::Message> m_MessageHistory;
};