#include "stdafx.h"

using namespace OsirisI::Resources;
using namespace OsirisI::Scripting;
using namespace OsirisI::Utilities;

namespace OsirisI {
    namespace Manager {
		std::map<long, IOsirisScript*> ScriptManager::scripts;

		ReturnState ScriptManager::Initialize() {
			ResourceManager::GetResourceDirectly("*:Scripts/Init/");
			return OS_OK;
		}

		IOsirisScript* ScriptManager::GetScript(long scriptName) {
			auto it = scripts.find(scriptName);
			if(it == scripts.end()) return nullptr;
			return it->second;
		}

		ReturnState ScriptManager::RegisterScript(OsirisI::Scripting::IOsirisScript* script) {
			if (GetScript(script->GetName().GetHash()) != nullptr) return OS_OBJECT_ALREADY_EXISTS;

			scripts.insert(std::pair<long, IOsirisScript*>(script->GetName().GetHash(), script));

			return OS_OK;
		}

		ReturnState ScriptManager::CreateScript(OsirisI::Utilities::HashedString scriptName, OsirisI::Resources::Resource* script, ScriptType type, int numberOfRuns, float intervalInSeconds) {
			switch (type) {
				case ScriptType::Js: {
					break;
				}
				case ScriptType::Lua: {
					LuaScript* lScript = new LuaScript(scriptName, ((ResourcePtr<char*>*)script)->GetValue(), numberOfRuns, intervalInSeconds);
					OS_CHECKSTATE(lScript->Initialize());
					scripts.insert(std::pair<long, IOsirisScript*>(scriptName.GetHash(), lScript));

					break;
				}
				case ScriptType::Python: {
					break;
				}
				default: {
					LOG_WARNING("Unknown script type: " << type);
					break;
				}
			}

			return OS_OK;
		}

		ReturnState ScriptManager::RemoveScript(OsirisI::Scripting::IOsirisScript* script) {
			if (GetScript(script->GetName().GetHash()) != nullptr) {
				scripts.erase(script->GetName().GetHash());
				return OS_OK;
			}

			return OS_OBJECT_NOT_FOUND;
		}

        ReturnState ScriptManager::Update(float delta) {
			ReturnState state = OS_OK;

			std::map<long, OsirisI::Scripting::IOsirisScript*>::iterator it = scripts.begin();
			for (; it != scripts.end(); ++it) 
			{
				ReturnState tmpState = it->second->Update(delta);
				if (tmpState != OS_OK) state = tmpState;				
			}
			
            return state;
        }

		ReturnState ScriptManager::Render() {
			ReturnState state = OS_OK;

			std::map<long, OsirisI::Scripting::IOsirisScript*>::iterator it = scripts.begin();
			for (; it != scripts.end(); ++it)
			{
				ReturnState tmpState = it->second->Render();
				if (tmpState != OS_OK) state = tmpState;
			}

			return state;
        }

		ReturnState ScriptManager::Release() {
			ReturnState state = OS_OK;

			std::map<long, OsirisI::Scripting::IOsirisScript*>::iterator it = scripts.begin();
			for (; it != scripts.end(); ++it)
			{
				ReturnState tmpState = it->second->Release();
				if (tmpState != OS_OK) state = tmpState;
			}

            return OS_OK;
        }
    }
}
