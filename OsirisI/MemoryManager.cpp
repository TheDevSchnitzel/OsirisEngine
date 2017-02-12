#include "stdafx.h"

using namespace OsirisI::Utilities;
using namespace OsirisI::Memory;

namespace OsirisI {
    namespace Manager {
        unsigned int MemoryManager::maxPools, MemoryManager::currPools, MemoryManager::poolSize, MemoryManager::maxSize;
        std::map<std::string, OsirisI::Memory::MemoryPool*> MemoryManager::memoryPools;
        
        ReturnState MemoryManager::Initialize(unsigned int maxPoolsP, unsigned int maxSizeP) {
            maxSize = maxSizeP;
            maxPools = maxPoolsP;
            RegisterPool(new MemoryPool(), "First");

            GetMemoryPool("First")->Initialize(maxSize / 2, 64);

            return OS_OK;
        }

        bool MemoryManager::RegisterPool(MemoryPool* memoryPool, std::string name) {
            if (memoryPools.find(name) != memoryPools.end()) return false;
            memoryPools.insert(std::pair<std::string, MemoryPool*>(name, memoryPool));
            return true;
        }

        MemoryPool* MemoryManager::GetMemoryPool(std::string name) {
            if (memoryPools.find(name) == memoryPools.end()) return 0;
            return memoryPools[name];
        }

        bool MemoryManager::FreePool(std::string name) {
            if (memoryPools.find(name) == memoryPools.end()) return false;
            memoryPools[name]->Release();
            memoryPools.erase(name);
            return true;
        }

        void* MemoryManager::Alloc(unsigned int size, const char* file, const char* function, long line) {
            std::map<std::string, MemoryPool*>::iterator it;
            MemoryPool* usedPool = 0;
            unsigned int memoryLoss = 0xFFFFFFFF;
            for ( it = memoryPools.begin(); it != memoryPools.end(); ++it ) {
                MemoryPool* currPool = it->second;
                if ( ( currPool->GetChunkSize() - ( size%currPool->GetChunkSize() ) ) < memoryLoss && currPool->GetFreeSize() >= size ) {
                    memoryLoss = currPool->GetChunkSize() - ( size%currPool->GetChunkSize() );
                    usedPool = currPool;
                }
            }
            if (usedPool == 0) return 0;

            void* ptr = usedPool->Alloc(size);
            if (ptr != 0) {
                LOG_DEBUG("MemoryManager: {[Ptr]: " << ptr << ", [Alloc]: " << size << ", [file]: " << file << ", [function]: " << function << ", [line]: " << line << "}");
                return ptr;
            }
            return 0;
        }

        void MemoryManager::Free(void* ptr) {
            std::map<std::string, MemoryPool*>::iterator it;
            for (it = memoryPools.begin(); it != memoryPools.end(); ++it) {
                if (it->second->Free(ptr)) break;
            }
        }

        bool MemoryManager::Release() {
            std::map<std::string, MemoryPool*>::iterator it;
            for (it = memoryPools.begin(); it != memoryPools.end(); ++it) {
                it->second->Release();
                delete it->second;
                it->second = nullptr;
            }
            memoryPools.clear();
            return true;
        }
    }
}

//void* operator new( size_t size ) {
//    if ( size == 0 ) {
//        throw std::bad_alloc();
//    }
//
//    return Manager::MemoryManager::Alloc(size);
//}
//void* operator new[] (size_t size) {
//    if ( size == 0 ) {
//        throw std::bad_alloc();
//    }
//
//    return Manager::MemoryManager::Alloc(size);
//}
//void operator delete( void* ptr ) {
//    Manager::MemoryManager::Free(ptr);
//}
//void operator delete[] (void* ptr) {
//    Manager::MemoryManager::Free(ptr);
//}
