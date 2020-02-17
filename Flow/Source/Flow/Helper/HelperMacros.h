#pragma once
#define CHECK_RETURN(Condition, ErrorMessage) if(Condition) { FLOW_ENGINE_ERROR(ErrorMessage); return; }
#define CHECK_RETURN_FALSE(Condition, ErrorMessage) if(Condition) { FLOW_ENGINE_ERROR(ErrorMessage); return false; }
#define CHECK_RETURN_ZERO(Condition, ErrorMessage) if(Condition) { FLOW_ENGINE_ERROR(ErrorMessage); return 0; }
#define CHECK_ASSERT(Condition, Message) assert(Condition && Message);