#pragma once
#define CHECK_RETURN(Condition, ErrorMessage) if(Condition) { FLOW_ENGINE_ERROR(ErrorMessage); return; }
#define CHECK_RETURN_FALSE(Condition, ErrorMessage) if(Condition) { FLOW_ENGINE_ERROR(ErrorMessage); return false; }
#define CHECK_RETURN_ZERO(Condition, ErrorMessage) if(Condition) { FLOW_ENGINE_ERROR(ErrorMessage); return 0; }
#define CHECK_RETURN_OBJECT(Condition, Object, ErrorMessage) if(Condition) { FLOW_ENGINE_ERROR(ErrorMessage); return Object; }
#define CHECK_ASSERT(Condition, Message) assert(Condition && Message);

#define IMGUI_GREEN ImVec4(0.0f, 1.0f, 0.0f, 1.0f)
#define IMGUI_RED ImVec4(1.0f, 0.0f, 0.0f, 1.0f)
#define IMGUI_YELLOW ImVec4(1.0f, 1.0f, 0.0f, 1.0f)
#define IMGUI_PURPLE ImVec4(0.55f, 0.0f, 0.55f, 1.0f)
#define IMGUI_ORANGE ImVec4(1.0f, 0.65f, 0.0f, 1.0f)