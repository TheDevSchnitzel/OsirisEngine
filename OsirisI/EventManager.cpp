#include "stdafx.h"

using namespace OsirisI::Events;

namespace OsirisI {
    namespace Manager {
        std::mutex EventManager::lock;
        std::map<long, std::list<OsirisI::Events::IEventListener*>> EventManager::eventMapping; //Long == HashedString.GetHash();
        std::map<long, bool> EventManager::queuedEventsAsync;
        std::deque<std::shared_ptr<OsirisI::Events::IEvent>> EventManager::queue;

        ReturnState EventManager::AddEventMapping(long eventNameHash, OsirisI::Events::IEventListener* listener) {
            lock.lock();
            if ( eventMapping.find(eventNameHash) == eventMapping.end() ) {
                eventMapping.insert(std::pair<long, std::list<OsirisI::Events::IEventListener*>>(eventNameHash, std::list<OsirisI::Events::IEventListener*>()));
                eventMapping[eventNameHash].push_back(listener);
            }
            else {
                std::list<OsirisI::Events::IEventListener*>::iterator findIter = std::find(eventMapping[eventNameHash].begin(), eventMapping[eventNameHash].end(), listener);
                if ( findIter == eventMapping[eventNameHash].end() ) {
                    eventMapping[eventNameHash].push_back(listener);
                }
                else {
                    lock.unlock();
                    return OS_EVENT_ALREADY_REGISTERED;
                }
            }
            lock.unlock();
            return OS_OK;
        }

        ReturnState EventManager::RemoveEventMapping(long eventNameHash) {
            lock.lock();
            if ( eventMapping.find(eventNameHash) == eventMapping.end() ) {
                lock.unlock();
                return OS_EVENT_NOT_REGISTERED;
            }
            eventMapping.erase(eventNameHash);
            lock.unlock();
            return OS_OK;
        }

        ReturnState EventManager::RemoveListenerMapping(OsirisI::Events::IEventListener* listener) {
            if ( listener == nullptr ) return OS_OBJECT_NOT_FOUND;

            lock.lock();
            std::map<long, std::list<OsirisI::Events::IEventListener*>>::iterator it = eventMapping.begin(), end = eventMapping.end();

            for ( ; it != end; ++it ) {
                std::list<OsirisI::Events::IEventListener*>::iterator itList = it->second.begin(), endList = it->second.end();
                for ( ; itList != endList; ++itList ) {
                    if ( ( ( OsirisI::Events::IEventListener* )*itList )->GetNameHash() == listener->GetNameHash() ) {
                        it->second.erase(itList);
                        break;
                    }
                }
            }
            lock.unlock();
            return OS_OK;
        }

		ReturnState EventManager::FireSyncEvent(std::shared_ptr<OsirisI::Events::IEvent> eventObj) {
            if ( eventObj == nullptr ) return OS_OBJECT_NOT_FOUND;

            lock.lock();
            if ( eventMapping.find(eventObj->GetNameHash()) == eventMapping.end() ) {
                lock.unlock();
                return OS_EVENT_NOT_REGISTERED;
            }

            std::list<OsirisI::Events::IEventListener*> mappingList = eventMapping[eventObj->GetNameHash()];
            lock.unlock();

            std::list<OsirisI::Events::IEventListener*>::const_iterator it = mappingList.begin(), end = mappingList.end();
            for ( ; it != end; ++it ) {
                ( ( OsirisI::Events::IEventListener* )*it )->HandleEvent(eventObj);
            }

            return OS_OK;
        }

		ReturnState EventManager::FireAsyncEvent(std::shared_ptr<OsirisI::Events::IEvent> eventObj) {
            if ( eventObj == nullptr ) return OS_OBJECT_NOT_FOUND;

            lock.lock();
            if ( eventMapping.find(eventObj->GetNameHash()) == eventMapping.end() ) {
                lock.unlock();
                return OS_EVENT_NOT_REGISTERED;
            }

            std::list<OsirisI::Events::IEventListener*> mappingList = eventMapping[eventObj->GetNameHash()];
            lock.unlock();

            std::list<OsirisI::Events::IEventListener*>::const_iterator it = mappingList.begin(), end = mappingList.end();
            for ( ; it != end; ++it ) {
                TaskManager::AddTask([=] () { ( ( OsirisI::Events::IEventListener* )*it )->HandleEvent(eventObj); });
            }
            return OS_OK;
        }

		ReturnState EventManager::FireQueueEvent(std::shared_ptr<OsirisI::Events::IEvent> eventObj, bool async) {
            if ( eventObj == nullptr ) return OS_OBJECT_NOT_FOUND;

            lock.lock();
            /*if ( queuedEventsAsync.find(eventObj->GetNameHash()) != queuedEventsAsync.end() ) {
                lock.unlock();
                return OS_OBJECT_ALREADY_QUEUED;
            }*/
            queue.push_back(std::shared_ptr<IEvent>(eventObj));
            queuedEventsAsync.insert(std::pair<long, bool>(eventObj->GetNameHash(), async));
            lock.unlock();
			return OS_OK;
        }

        ReturnState EventManager::ProcessQueue() {
            lock.lock();
            while ( !queue.empty() ) {
                std::shared_ptr<OsirisI::Events::IEvent> eventObj = queue.front();
                queue.pop_front();

                bool async = queuedEventsAsync[eventObj->GetNameHash()];
                queuedEventsAsync.erase(eventObj->GetNameHash());

                if ( async ) {
                    lock.unlock();
                    FireAsyncEvent(eventObj);
                    lock.lock();
                }
                else {
                    lock.unlock();
                    FireSyncEvent(eventObj);
                    lock.lock();
                }
            }

            lock.unlock();
            return OS_OK;
        }
    }
}
