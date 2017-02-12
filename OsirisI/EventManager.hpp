#ifndef EVENTMANAGER_HPP_
#define EVENTMANAGER_HPP_

namespace OsirisI {
	namespace Manager {
		class EventManager {
			private:
                static std::mutex lock;
				static std::map<long, std::list<OsirisI::Events::IEventListener*>> eventMapping; //Long == HashedString.GetHash();
				static std::deque<std::shared_ptr<OsirisI::Events::IEvent>> queue;
				static std::map<long, bool> queuedEventsAsync;

			public:	
				static ReturnState AddEventMapping(long eventNameHash, OsirisI::Events::IEventListener* listener);
				static ReturnState RemoveEventMapping(long eventNameHash);
				static ReturnState RemoveListenerMapping(OsirisI::Events::IEventListener* listener);

				static ReturnState FireSyncEvent(std::shared_ptr<OsirisI::Events::IEvent> eventObj);
				
				/**
				Creates a std::shared_ptr<OsirisI::Events::IEvent> internally and calls FireSyncEvent again.
				This function should only be used when the event object is not used in the calling code.
				Example: OS_CHECKSTATE(EventManager::FireSyncEvent(new IEvent()));
				*/
				static ReturnState FireSyncEvent(OsirisI::Events::IEvent* eventObj) {
					return FireSyncEvent(std::shared_ptr<OsirisI::Events::IEvent>(eventObj));
				}

				static ReturnState FireAsyncEvent(std::shared_ptr<OsirisI::Events::IEvent> eventObj);

				/**
				Creates a std::shared_ptr<OsirisI::Events::IEvent> internally and calls FireAsyncEvent again.
				This function should only be used when the event object is not used in the calling code.
				Example: OS_CHECKSTATE(EventManager::FireAsyncEvent(new IEvent()));
				*/
				static ReturnState FireAsyncEvent(OsirisI::Events::IEvent* eventObj) {
					return FireAsyncEvent(std::shared_ptr<OsirisI::Events::IEvent>(eventObj));
				}

				static ReturnState FireQueueEvent(std::shared_ptr<OsirisI::Events::IEvent> eventObj, bool async = false);
				
				/**
				Creates a std::shared_ptr<OsirisI::Events::IEvent> internally and calls FireQueueEvent again.
				This function should only be used when the event object is not used in the calling code.
				Example: OS_CHECKSTATE(EventManager::FireQueueEvent(new IEvent()));
				*/
				static ReturnState FireQueueEvent(OsirisI::Events::IEvent* eventObj, bool async = false) {
					return FireQueueEvent(std::shared_ptr<OsirisI::Events::IEvent>(eventObj), async);
				}

				static ReturnState ProcessQueue();
		};
	}
}
#endif /* EVENTMANAGER_HPP_ */
