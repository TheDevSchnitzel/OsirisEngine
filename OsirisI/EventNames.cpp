#include "stdafx.h"

using namespace OsirisI::Events;

const OsirisI::Utilities::HashedString OsirisI_MouseInputEvent::eventName("OsirisI_MouseInputEvent");
const OsirisI::Utilities::HashedString OsirisI_KeyboardInputEvent::eventName("OsirisI_KeyboardInputEvent");

const OsirisI::Utilities::HashedString OsirisI_SystemEvent::eventName("OsirisI_SystemEvent");

const OsirisI::Utilities::HashedString OsirisI_NetworkDataEvent::eventName("OsirisI_NetworkDataEvent");
const OsirisI::Utilities::HashedString OsirisI_NetworkStatusEvent::eventName("OsirisI_NetworkStatusEvent");
const OsirisI::Utilities::HashedString OsirisI_NetworkErrorEvent::eventName("OsirisI_NetworkErrorEvent");

const OsirisI::Utilities::HashedString OsirisI_LuaScriptEvent::eventName("OsirisI_LuaScriptEvent");
const OsirisI::Utilities::HashedString OsirisI_ScriptEvent::eventName("OsirisI_ScriptEvent");
