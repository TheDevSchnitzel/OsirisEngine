#include "stdafx.h"

using namespace OsirisI::Utilities;
using namespace LuaPlus;

namespace OsirisI {
    namespace Scripting {
		LuaScript::LuaScript(HashedString scriptName, char* script, int numberOfRuns, float intervalInSeconds) {
			/*int codeLength = 0;
			int scriptID = 0;
			long timestampOfCreation = 0;
			char author[20];
			char signhash[32];
			char version[15];
			char scripttype = -1;

			memcpy_s(&codeLength, 4, script, 4);
			memcpy_s(&scriptID, 4, script + 0x4, 4);
			memcpy_s(&author, 20, script + 0x8, 20);
			memcpy_s(&timestampOfCreation, 8, script + 0x1C, 8);
			memcpy_s(&signhash, 32, script + 0x25, 32);
			memcpy_s(&version, 15, script + 0x45, 15);
			memcpy_s(&scripttype, 1, script + 0x54, 1);

			this->script = new char[codeLength + 1];
			this->script[codeLength] = 0;
			memcpy_s(this->script, codeLength, script + 0x55, codeLength);*/
			this->script = script;
			
			this->numberOfRuns = 0;
			this->remainingRuns = numberOfRuns;
			this->intervalInSeconds = intervalInSeconds;
			this->currentIntervalState = intervalInSeconds;
			this->scriptName = scriptName;
        }

        LuaScript::~LuaScript() {
        }

		int LuaScript::Lua_AtPanic(lua_State *state) {
			lua_getglobal(state, "OS_SCRIPT_NAME");
			LOG_ERROR("LuaScript " << lua_tostring(state, -1) << " paniced!");

			lua_getglobal(state, "OS_NUMBER_OF_RUNS");
			LOG_NESTED_NEW("NumberOfRuns: " << lua_tonumber(state, -1));

			lua_getglobal(state, "OS_REMAINING_RUNS");
			LOG_NESTED("RemainingRuns: " << lua_tonumber(state, -1));

			lua_getglobal(state, "OS_INTERVAL_IN_SEC");
			LOG_NESTED("IntervalInSeconds: " << lua_tonumber(state, -1));

			lua_getglobal(state, "OS_CURR_INTERVAL_STATE");
			LOG_NESTED("CurrentIntervalState: " << lua_tonumber(state, -1));

			return 0;
		}

		ReturnState LuaScript::Initialize() {
			this->state->AtPanic(&OsirisI::Scripting::LuaScript::Lua_AtPanic);

			lua_State* st = state->GetCState();
			static const luaL_Reg lualibs[] =
			{
				{ "math", luaopen_math },
				{ NULL, NULL }
			};

			const luaL_Reg *lib = lualibs;
			for (; lib->func != NULL; lib++) {
				lib->func(st);
				lua_settop(st, 0);
			}

			LOG_INFO("LUA-Init[" << this->GetName().GetString() << "]: " << this->state->DoString(this->script));
			const char * str = lua_tostring(st, -1);
			if (str != nullptr) {
				LOG_ERROR(str);
				if (HelperFunctions::StringContains(str, "'ý'")) {
					LOG_NESTED_NEW("Maybe you forgot the 0x00[NULL/Nil] at the end of your scriptfile?");
				}
				return OS_INITIALIZATION_FAILED;
			}
					
			this->state->GetGlobals().SetString("OS_SCRIPT_NAME", this->GetName().GetString().c_str());
			this->state->GetGlobals().SetNumber("OS_SCRIPT_NAME_HASH", this->GetName().GetHash());
			this->state->GetGlobals().SetNumber("OS_NUMBER_OF_RUNS", this->numberOfRuns);
			this->state->GetGlobals().SetNumber("OS_REMAINING_RUNS", this->remainingRuns);
			this->state->GetGlobals().SetNumber("OS_INTERVAL_IN_SEC", this->intervalInSeconds);
			this->state->GetGlobals().SetNumber("OS_CURR_INTERVAL_STATE", this->currentIntervalState);
						
			try {
				LuaFunction<int> initialize = state->GetGlobal("Initialize");
				OS_CHECKSTATE(initialize());
			}
			catch (...) {
				LOG_ERROR("Call to Lua_Initialize failed");
				return OS_INITIALIZATION_FAILED;
			}

            return OS_OK;
        }

        ReturnState LuaScript::Update(float delta) {			
            if ( intervalInSeconds > 0 ) {
				currentIntervalState -= delta;
				this->state->GetGlobals().SetNumber("OS_CURR_INTERVAL_STATE", this->currentIntervalState);
            }
						
			LuaFunction<int> update = state->GetGlobal("Update");
			OS_CHECKSTATE(update(delta));

            return OS_OK;
        }

        ReturnState LuaScript::Render() {
			if (remainingRuns > 0 && currentIntervalState <= 0) {
				currentIntervalState = intervalInSeconds;

                numberOfRuns++;
				remainingRuns--;

				this->state->GetGlobals().SetNumber("OS_REMAINING_RUNS", this->remainingRuns);
				this->state->GetGlobals().SetNumber("OS_NUMBER_OF_RUNS", this->numberOfRuns);
				this->state->GetGlobals().SetNumber("OS_CURR_INTERVAL_STATE", this->currentIntervalState);

				LuaFunction<int> render = state->GetGlobal("Render");
				OS_CHECKSTATE(render());
            }

            return OS_OK;
        }

		ReturnState LuaScript::Release() {
			LuaFunction<int> release = state->GetGlobal("Release");
			OS_CHECKSTATE(release());

			return IOsirisScript::Release();
        }
    }
}
