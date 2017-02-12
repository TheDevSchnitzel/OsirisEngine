#ifndef SCRIPTMANAGER_HPP_
#define SCRIPTMANAGER_HPP_

namespace OsirisI {
	namespace Scripting {
		/*
		Forward declaration due to usage of the ScriptManager 
		in the Release function of IOsirisScript
		*/

		class IOsirisScript;
		class LuaScript;
		class PythonScript;
		class JsScript;
	}

	namespace Manager {
        enum ScriptType {Lua = 0, Js, Python};
        
		class ScriptManager {
			private:
                static std::mutex lock;
                static std::map<long, OsirisI::Scripting::IOsirisScript*> scripts;

		public:
			static OsirisI::Scripting::IOsirisScript* GetScript(long scriptName);

			static ReturnState CreateScript(OsirisI::Utilities::HashedString scriptName, OsirisI::Resources::Resource* script, ScriptType type, int numberOfRuns = 1, float intervalInSeconds = 0);
			static ReturnState RegisterScript(OsirisI::Scripting::IOsirisScript* script);
			static ReturnState RemoveScript(OsirisI::Scripting::IOsirisScript* script);

			static ReturnState Initialize();
			static ReturnState Update(float delta);
			static ReturnState Render();
			static ReturnState Release();
		};
	}
}

#endif /* SCRIPTMANAGER_HPP_ */
