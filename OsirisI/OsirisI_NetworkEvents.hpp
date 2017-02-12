#ifndef OSIRISI_NETWORKEVENTS_HPP_
#define OSIRISI_NETWORKEVENTS_HPP_

namespace OsirisI {
	namespace Events {
		struct OsirisI_NetworkDataEvent : public IEvent {
			private:

			protected:
                NetworkFrame frame;
				
			public:
                static const OsirisI::Utilities::HashedString eventName;

                OsirisI_NetworkDataEvent(NetworkFrame pFrame) : frame(pFrame) {
                }

                ~OsirisI_NetworkDataEvent() {
                }
				
				NetworkFrame GetNetworkFrame() {
					return this->frame;
				}

                OsirisI::Utilities::HashedString GetName() {
                    return eventName;
                }

                long GetNameHash() {
                    return eventName.GetHash();
                }
        };

        enum class OsirisI_NetworkStatus {SERVER_STARTED, SERVER_STOPPED, CLIENT_JOINED, CLIENT_LEFT,CLIENT_STARTED,CLIENT_STOPPED};
		struct OsirisI_NetworkStatusEvent : public IEvent {
			private:

			protected:
                OsirisI_NetworkStatus status;
				
			public:
                static const OsirisI::Utilities::HashedString eventName;

                OsirisI_NetworkStatusEvent(OsirisI_NetworkStatus pStatus) : status(pStatus) {
                }

                ~OsirisI_NetworkStatusEvent() {
                }
				
                OsirisI::Utilities::HashedString GetName() {
                    return eventName;
                }

                long GetNameHash() {
                    return eventName.GetHash();
                }
        };

        enum class OsirisI_NetworkErrors {DAMAGED_FRAME};
		struct OsirisI_NetworkErrorEvent : public IEvent {
			private:

			protected:
                OsirisI_NetworkErrors error;
				
			public:
                static const OsirisI::Utilities::HashedString eventName;

                OsirisI_NetworkErrorEvent(OsirisI_NetworkErrors pError) : error(pError) {
                }

                ~OsirisI_NetworkErrorEvent() {
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

#endif /* OSIRISI_NETWORKEVENTS_HPP_ */
