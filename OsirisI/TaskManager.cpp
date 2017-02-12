#include "stdafx.h"

using namespace OsirisI::Threading;

namespace OsirisI {
	namespace Manager {
		OsirisI::Threading::ThreadPool* TaskManager::threadPool;

		ReturnState TaskManager::Initialize() {
			threadPool = new OsirisI::Threading::ThreadPool();
			return threadPool->Initialize(0);
		}

		void TaskManager::AddTask(std::function<void()> func){
			threadPool->AddToQueue(func);
		}

		ReturnState TaskManager::Release() {		
			return threadPool->Release();
		}

        std::thread TaskManager::StartThread(std::function<void()> func) {
            return std::thread(func);
        }
	}
}
