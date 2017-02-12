#ifndef LUASCRIPT_HPP_
#define LUASCRIPT_HPP_

namespace OsirisI {
    namespace Scripting {
        class LuaScript : public IOsirisScript {
			private:
				LuaPlus::LuaStateOwner state;
				static int Lua_AtPanic(lua_State *state);

			protected:

			public:
				LuaScript(OsirisI::Utilities::HashedString scriptName, char* script, int numberOfRuns = 1, float intervalInSeconds = 0);
				~LuaScript();

                ReturnState Initialize();
                ReturnState Update(float delta);
                ReturnState Render();
                ReturnState Release();
        };
    }
}

#endif /* LUASCRIPT_HPP_ */
