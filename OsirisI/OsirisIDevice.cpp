#include "stdafx.h"

using namespace OsirisI::Events;
using namespace OsirisI::Manager;
using namespace OsirisI::Utilities;
using namespace OsirisI::Graphics;
using namespace OsirisI::Graphics::Actors;
using namespace OsirisI::Graphics::Devices;
using namespace OsirisI::Graphics::Renderer;
using namespace OsirisI::Resources;
using namespace OsirisI::Resources::Loader;
using namespace std::chrono;

namespace OsirisI {
	OsirisI_DeviceType OsirisIDevice::DeviceType;
	bool OsirisIDevice::stopMainLoop;
	OsirisIDevice* OsirisIDevice::g_OsirisIDevice;

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
            case WM_DESTROY: 
                PostQuitMessage(0);
                return 0;
                break;
            
            default: 
                //Any other messages send to the default message handler as our application won't make use of them.
                return DefWindowProcA(hWnd, message, wParam, lParam);
                break;
        }
    }
    
	OsirisIDevice::OsirisIDevice(int width, int height, bool windowed, bool vsync) {
		if (Logger::Initialize(OSIRIS_LOGGER_FILE, LOG_RANGE_INFO) != OS_OK) {
			throw new std::exception("Initialization of Logger failed!");
			return;
		}
		else {
			if (EngineConfiguration::Initialize() != OS_OK)	{
				throw new std::exception("Initialization of EngineConfiguration failed!");
				return;
			}
			else {
				g_OsirisIDevice = this;

				this->vsync = vsync;
				this->windowed = windowed;
				this->width = width;
				this->height = height;
				stopMainLoop = false;

				server = new Networking::OsirisI_Server();
				
				MemoryManager::Initialize(2, 10 * MiB);

				int* ar = (int*)OS_NEW(sizeof(int) * 100000);

				for (int i = 0; i < 100000; i++)
					ar[i] = i;
				OS_FREE(ar);

				MemoryManager::GetMemoryPool("First")->DumpMemory();
			}
		}
	}

	OsirisIDevice::~OsirisIDevice() {
		g_OsirisIDevice = nullptr;
	}
    
	bool OsirisIDevice::InitializeDX(std::string windowName) {
		OsirisIDevice::DeviceType = OsirisI_DeviceType::DX11;
		this->graphicDevice = new Direct3DDevice(vsync, width, height, windowed);
		this->renderer = new DirectX11Renderer();

		((DirectX11Renderer*)this->renderer)->SetDirect3DDevice((Direct3DDevice*)this->graphicDevice);
        
		LOG_INFO("InitializeDX");

		ReturnState state = ((Direct3DDevice*)this->graphicDevice)->Initialize(WndProc, windowName);
		if(state != 0) {
			LOG_ERROR("Error while initializing device - Code: " << state);
			return false;
		}
		
		SetupInternal();
        EventManager::FireSyncEvent(new OsirisI_SystemEvent(OsirisSystemState::PostInitialize, "", state));

		return (state == 0);
	}

	bool OsirisIDevice::InitializeGL(std::string windowName) {	
		OsirisIDevice::DeviceType = OsirisI_DeviceType::OpenGL;
		ReturnState state = 0;		
		state = Logger::Initialize(OSIRIS_LOGGER_FILE, LOG_RANGE_INFO);
		LOG_INFO("InitializeGL");
		//if(state == 0) state = this->hwnd->Initialize(proc, windowName);
		//if(state == 0) state = this->d3dDevice->Initialize(this->hwnd);
		//g_graphicDevice = dev;
		return 1;
	}
	
	bool OsirisIDevice::SetupInternal() {
		stopMainLoop = false;

		#pragma region Manager initialization
            IEventListener* listener = new OsirisIEngineEventListener();
            EventManager::AddEventMapping(OsirisI_SystemEvent::eventName.GetHash(), listener);
            EventManager::AddEventMapping(OsirisI_KeyboardInputEvent::eventName.GetHash(), listener);
            EventManager::FireSyncEvent(new OsirisI_SystemEvent(OsirisSystemState::PreInitialize));

			InputManager::Initialize();
			TaskManager::Initialize(); //Creates ThreadPool with NumCores-1 threads
			SceneManager::Initialize(this->renderer);

			ScriptManager::Initialize();
		#pragma endregion

		#pragma region DefaultLoader
            LoaderManager::RegisterLoader<ITexture>(new DXTextureLoader());
            LoaderManager::RegisterLoader<IGameModel>(new WaveFrontObjectModelLoader());
            LoaderManager::RegisterLoader<Font>(new FontLoader());
		#pragma endregion

           // server->Initialize(15, 52439);            
			
		return true;
	}
    
	bool OsirisIDevice::Run() {
		MSG msg;
		
		int averageFPSCounter = 0;
		float frameTime = 0, updateTime = 0;
		bool useScripts = (HelperFunctions::ToLower(EngineConfiguration::GetProperty("Scripts", "UseScripts")) == "true") ? true : false;
		Timer timer;
		timer.StartTimer();

		while (!stopMainLoop) {
			lock.lock();

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				//if (InputManager::HandleInput(msg.message, msg.wParam, msg.lParam) != OS_OK) {
					TranslateMessage(&msg);

					DispatchMessage(&msg);

					if (msg.message == WM_QUIT || msg.message == WM_DESTROY) {
						//Process event queue one last time
						if (EventManager::ProcessQueue() != OS_OK) {
							LOG_ERROR("Error while processing event queue! -> Processing aborted!");
						}

						EventManager::FireSyncEvent(new OsirisI_SystemEvent(OsirisSystemState::Shutdown));
						break;
					}
				//}
            }
            else {
				#pragma region Time measured area (Update/Render)
                auto updateTimeStart = std::chrono::high_resolution_clock::now();
				double realFrameTime = timer.GetFrameTime();

				InputManager::DetectInput(realFrameTime);
				SceneManager::Update(realFrameTime);
				GameStateManager::Update(realFrameTime);
				
				if (useScripts)ScriptManager::Update(realFrameTime);

                auto frameTimeStart = std::chrono::high_resolution_clock::now();
                OS_CHECKSTATE_THROW(SceneManager::Render());
				auto frameTimeEnd = std::chrono::high_resolution_clock::now();
				OS_CHECKSTATE(ScriptManager::Render());

                frameTime += duration_cast<duration<float>>(frameTimeEnd - frameTimeStart).count(); //Frame duration in seconds
                updateTime += duration_cast<duration<float>>(frameTimeStart - updateTimeStart).count(); //Update duration in seconds

                if (averageFPSCounter++ == 1000) {
					LOG_INFO("Frametime per 1000 frames: " << frameTime / 1000);
					LOG_INFO("Updatetime per 1000 frames: " << updateTime / 1000);
					LOG_INFO("TimerTime: " << realFrameTime);
					
                    frameTime = 0, updateTime = 0, averageFPSCounter = 0;
                }
				#pragma endregion
                
                if(EventManager::ProcessQueue() != OS_OK) {
                    LOG_ERROR("Error while processing event queue! -> Processing aborted!");
                }
			}
			lock.unlock();
		}
		return true;
	}

	bool OsirisIDevice::Recover() {
		return true;
	}

	bool OsirisIDevice::Release() {
		stopMainLoop = true;

		if(this->graphicDevice != nullptr) {
			this->graphicDevice->Release();
			delete this->graphicDevice;
		}

        server->Stop();

		return true;
	}
}
