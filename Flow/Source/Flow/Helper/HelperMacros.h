#pragma once
#define CHECK_RETURN(Condition, ErrorMessage) if(Condition) { FLOW_ENGINE_ERROR(ErrorMessage); return; }