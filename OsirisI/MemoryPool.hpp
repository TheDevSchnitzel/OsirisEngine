#ifndef MEMORYPOOL_HPP_
#define MEMORYPOOL_HPP_

namespace OsirisI {
    namespace Memory {
        struct MemoryChunk {
            public:
            MemoryChunk() {
                usedSize = 0;
                data = 0;
            }

            unsigned int usedSize;
            unsigned char* data;
        };
        class MemoryPool {
            private:
            unsigned char* heap;
            MemoryChunk* chunks;
            unsigned int poolSize;
            unsigned int usedSize;
            unsigned int numOfUsedChunks;
            unsigned int numOfChunks;
            unsigned int chunkSize;
            std::map<void*, unsigned int> memSizeMapping;
            std::map<void*, unsigned int> memChunkMapping;
            bool isReleased;

            public:
            MemoryPool();
            ~MemoryPool();

            bool Initialize(unsigned int heapSize, unsigned int chunkSize);
            bool Release();

            void* Alloc(size_t size);
            bool Free(void* ptr);

            void DumpMemory();

            unsigned int GetChunkSize();
            unsigned int GetPoolSize();
            unsigned int GetUsedSize();
            unsigned int GetFreeSize();
        };
    }
}

#endif /* MEMORYPOOL_HPP_ */
