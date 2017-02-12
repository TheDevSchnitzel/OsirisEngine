#ifndef OSIRISI_SCRIPTEVENTS_HPP_
#define OSIRISI_SCRIPTEVENTS_HPP_

namespace OsirisI {
    namespace Events {
        struct OsirisI_ScriptEvent : public IEvent {
            public:
                static const OsirisI::Utilities::HashedString eventName;

                OsirisI_ScriptEvent() {
                }

                ~OsirisI_ScriptEvent() {
                }

                OsirisI::Utilities::HashedString GetName() {
                    return eventName;
                }

                long GetNameHash() {
                    return eventName.GetHash();
                }
        };
        
        struct OsirisI_LuaScriptEvent : public IEvent {
            private:

            protected:
                lua_State *luaState = nullptr;

            public:
                static const OsirisI::Utilities::HashedString eventName;

                OsirisI_LuaScriptEvent() {
                }

                ~OsirisI_LuaScriptEvent() {
                }
            
                OsirisI::Utilities::HashedString GetName() {
                    return eventName;
                }

                long GetNameHash() {
                    return eventName.GetHash();
                }
        };
    }
}

#endif /* OSIRISI_SCRIPTEVENTS_HPP_ */
