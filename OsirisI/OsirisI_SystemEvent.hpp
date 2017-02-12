#ifndef OSIRISISYSTEMEVENT_HPP_
#define OSIRISISYSTEMEVENT_HPP_

namespace OsirisI {
	namespace Events {
        enum class OsirisSystemState { PreInitialize = 0, PostInitialize, PreRelease, PostRelease, Shutdown };
		struct OsirisI_SystemEvent : public IEvent {
			private:

			protected:
				std::string msg;
				ReturnState code;
                OsirisSystemState systemState;
				
			public:
                static const OsirisI::Utilities::HashedString eventName;

                OsirisI_SystemEvent(OsirisSystemState state, std::string strMSG = "", ReturnState stateCode = OS_OK) : systemState(state), msg(strMSG), code(stateCode) {
                }

                ~OsirisI_SystemEvent() {
                }
				
				std::string GetMSG() {
					return this->msg;
				}

				ReturnState GetCode() {
					return this->code;
				}

                OsirisSystemState GetState() {
                    return this->systemState;
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

#endif /* OSIRISISYSTEMEVENT_HPP_ */
