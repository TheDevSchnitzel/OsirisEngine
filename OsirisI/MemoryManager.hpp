#ifndef MEMORYMANAGER_HPP_
#define MEMORYMANAGER_HPP_

namespace OsirisI {
    namespace Manager {
        class MemoryManager {
            private:
                static unsigned int maxPools, currPools, poolSize, maxSize;
                static std::map<std::string, OsirisI::Memory::MemoryPool*> memoryPools;

            public:
                static ReturnState Initialize(unsigned int maxPoolsP, unsigned int maxSizeP);

                static bool RegisterPool(OsirisI::Memory::MemoryPool* memoryPool, std::string name);
                static OsirisI::Memory::MemoryPool* GetMemoryPool(std::string name);
                static bool FreePool(std::string name);

                static void* Alloc(unsigned int size, const char* file = "", const char* function = "", long line = 0);
                static void Free(void* ptr);

                static bool Release();
        };
    }
}

#define USE_MEM_POOL 0

#define OS_NEW(size) ((USE_MEM_POOL == 1) ? OsirisI::Manager::MemoryManager::Alloc(size, __FILE__, __FUNCTION__, __LINE__) : new char[size]) 


#define OS_FREE(ptr) ((USE_MEM_POOL == 1) ? OsirisI::Manager::MemoryManager::Free(ptr) : delete[] ptr) 

//
//void* operator new( size_t size );
//void* operator new[](size_t size);
//void operator delete( void* ptr );
//void operator delete[](void* ptr);
#endif /* MEMORYMANAGER_HPP_ */
