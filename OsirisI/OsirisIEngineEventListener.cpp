#include "stdafx.h"

using namespace OsirisI::Utilities;
using namespace OsirisI::Manager;

namespace OsirisI {
    namespace Events {
        OsirisIEngineEventListener::OsirisIEngineEventListener() {
            registeredEvents.push_back(OsirisI_SystemEvent::eventName.GetHash());
            registeredEvents.push_back(OsirisI_KeyboardInputEvent::eventName.GetHash());
        }
        OsirisIEngineEventListener::~OsirisIEngineEventListener() {
        }

        ReturnState OsirisIEngineEventListener::Initialize() {
            return OS_OK;
        }

        ReturnState OsirisIEngineEventListener::HandleEvent(std::shared_ptr<IEvent> eventObj) {
            if (eventObj->GetNameHash() == OsirisI_SystemEvent::eventName.GetHash()) {
				std::shared_ptr<OsirisI_SystemEvent> cEvent = std::dynamic_pointer_cast<OsirisI_SystemEvent>(eventObj);
                ReturnState state = OS_OK;

                switch(cEvent->GetState()) {
                    case OsirisSystemState::PreInitialize:
                        LOG_INFO("Engine is initializing...");
                        break;
                    case OsirisSystemState::PostInitialize:
                        LOG_INFO("Engine properly initialized...");
                        break;
                    case OsirisSystemState::PreRelease:
						LOG_INFO("Engine pre release...");
                        break;
                    case OsirisSystemState::PostRelease:
						LOG_INFO("Engine post release...");
                        break;
                    case OsirisSystemState::Shutdown: 
						LOG_INFO("Engine is shutting down...");

						{   // acquire lock
							//OsirisIDevice::g_OsirisIDevice->lock.lock();
							//std::lock_guard<std::mutex> lockG(OsirisIDevice::g_OsirisIDevice->lock, std::adopt_lock);

							state = TaskManager::Release();
							if (state == OS_OK) {
								LOG_NESTED_NEW("TaskManager -> Released");
							}
							else {
								LOG_NESTED_NEW("[CRITICAL] TaskManager -> Code: 0x" << std::hex << state);
							}

							state = SceneManager::Release();
							if (state == OS_OK) {
								LOG_NESTED_NEW("SceneManager -> Released");
							}
							else {
								LOG_NESTED_NEW("[CRITICAL] SceneManager -> Code: 0x" << std::hex << state);
							}

							state = GameStateManager::Release();
							if (state == OS_OK) {
								LOG_NESTED_NEW("GameStateManager -> Released");
							}
							else {
								LOG_NESTED_NEW("[CRITICAL] GameStateManager -> Code: 0x" << std::hex << state);
							}

							state = ResourceManager::Release();
							if (state == OS_OK) {
								LOG_NESTED_NEW("ResourceManager -> Released");
							}
							else {
								LOG_NESTED_NEW("[CRITICAL] ResourceManager -> Code: 0x" << std::hex << state);
							}

							state = OsirisIDevice::g_OsirisIDevice->Release();
							if (state == OS_OK) {
								LOG_NESTED_NEW("OsirisIDevice -> Released");
							}
							else {
								LOG_NESTED_NEW("[CRITICAL] OsirisIDevice -> Code: 0x" << std::hex << state);
							}

							LOG_INFO("Shutdown complete!");

							Logger::Release();
						}

                        break;

                    default:
                        return OS_OK;
                }
            }
            else if(eventObj->GetNameHash() == OsirisI_KeyboardInputEvent::eventName.GetHash()) {
				std::shared_ptr<OsirisI_KeyboardInputEvent> e = std::dynamic_pointer_cast<OsirisI_KeyboardInputEvent>(eventObj);
				std::string eventName = ((HelperFunctions::IsTypeOf_SharedPtr<OsirisI_KeyboardInputEvent_KeyDown>(e)) ? "OsirisI_KeyboardInputEvent_KeyDown" : ((HelperFunctions::IsTypeOf_SharedPtr<OsirisI_KeyboardInputEvent_KeyPressed>(e)) ? "OsirisI_KeyboardInputEvent_KeyPressed" : ((HelperFunctions::IsTypeOf_SharedPtr<OsirisI_KeyboardInputEvent_KeyUp>(e)) ? "OsirisI_KeyboardInputEvent_KeyUp" : "UNKNOWN EVENT")));

				LOG_INFO(eventName << ": " << (int)e->GetChar() << ", " << KeyboardKeysNames[(int)e->GetKey1()] << ", " << KeyboardKeysNames[(int)e->GetKey2()] << ", " << KeyboardKeysNames[(int)e->GetKey3()]);
				if (e->GetKey1() == KeyboardKeys::F1) {
                    /*
                    Sync event count:
                    Async event count:
                    Queued event count:

                    Mouse (X/Y):

                    FPS/AFT: 
                    */
                }
				else if (e->GetKey1() == KeyboardKeys::M) {
                    LOG_INFO("M pressed....");                    
                    EventManager::FireSyncEvent(new OsirisI_SystemEvent(OsirisSystemState::Shutdown));
                }
            }
            else {
                return OS_EVENT_NOT_HANDLED;
            }
            return OS_OK; //If nothing unexpected happened return OS_OK;
            /*
            std::list<long>::iterator it = registeredEvents.begin();
            for(; it != registeredEvents.end(); ++it) {
            if(*it == eventObj.GetName()) {
            return OS_OK
            }
            }
            return OS_EVENT_NOT_HANDLED;*/
        }
    }
}
