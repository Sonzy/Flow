#pragma once
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/misc/cpp/imgui_stdlib.h"

namespace ImHelp
{
    static int InputTextCallback(ImGuiInputTextCallbackData* data)
    {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            // Resize string callback
            std::string* str = (std::string*)data->UserData;
            IM_ASSERT(data->Buf == str->c_str());
            str->resize(data->BufTextLen);
            data->Buf = (char*)str->c_str();
        }

        return ((std::string*)data->UserData)->size();
    }

};