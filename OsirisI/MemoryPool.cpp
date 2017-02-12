#include "stdafx.h"

namespace OsirisI {
    namespace Memory {

        MemoryPool::MemoryPool() {
            heap = 0;
            chunks = 0;
            poolSize = 0, usedSize = 0, numOfUsedChunks = 0, numOfChunks = 0, chunkSize = 0;
            isReleased = false;
        }

        MemoryPool::~MemoryPool() {
            if (!isReleased) {
                Release();
            }
        }

        bool MemoryPool::Initialize(unsigned int heapSize, unsigned int chunkSize) {
            if (heapSize%chunkSize != 0) return false;

            this->heap = new unsigned char[heapSize];
            memset(this->heap, 0, heapSize);

            this->poolSize = heapSize;
            this->chunkSize = chunkSize;
            this->numOfChunks = heapSize / chunkSize;
            this->chunks = new MemoryChunk[this->numOfChunks];
            return OS_OK;
        }

        void* MemoryPool::Alloc(size_t size) {
            if ( GetFreeSize() < size ) return NULL;

            size_t neededSize = size;
            void* startPtr = 0;
            unsigned int followingsFound = 0, startChunk = 0, neededChunks = ( size + this->chunkSize - ( size%this->chunkSize ) ) / this->chunkSize;
            bool freeChunksFound = false;

            for ( unsigned int i = 0; i < this->numOfChunks; i++ ) {
                if ( followingsFound == neededChunks ) {
                    startChunk = i - neededChunks;
                    freeChunksFound = true;
                    break;
                }

                if ( this->chunks[i].usedSize == 0 ) {
                    followingsFound++;
                    continue;
                }

                followingsFound = 0;
            }

            if ( !freeChunksFound ) return NULL;

            for ( unsigned int i = startChunk; i < ( startChunk + neededChunks ); i++ ) {
                this->chunks[i].usedSize = ( ( neededSize <= this->chunkSize ) ? neededSize : this->chunkSize );
                this->chunks[i].data = &( this->heap[i*this->chunkSize] );
                this->usedSize += this->chunkSize;
                this->numOfUsedChunks++;
                if ( startPtr == 0 ) startPtr = this->chunks[i].data;
            }
            this->memSizeMapping.insert(std::pair<void*, unsigned int>(startPtr, ( neededChunks*this->chunkSize )));
            this->memChunkMapping.insert(std::pair<void*, unsigned int>(startPtr, startChunk));
            return startPtr;
        }

        bool MemoryPool::Free(void* ptr) {
            if ( memChunkMapping.find(ptr) == memChunkMapping.end() ) return false;

            memset(ptr, 0, this->memSizeMapping[ptr]); //Setze Speicher auf 0

            unsigned int start = memChunkMapping[ptr], end = ( start + ( memSizeMapping[ptr] / this->chunkSize ) );
            for ( ; start < end; start++ ) {
                MemoryChunk* chunk = &this->chunks[start];
                this->usedSize -= chunk->usedSize;
                this->numOfUsedChunks--;
                this->chunks[start].data = 0;
                this->chunks[start].usedSize = 0;
            }
            return true;
        }

        bool MemoryPool::Release() {
            OS_SAFEDELETE(this->chunks);
            OS_SAFEDELETE(this->heap);
            this->isReleased = true;
            return true;
        }

        void MemoryPool::DumpMemory() {
            std::ofstream file("MemDump.bin", std::ofstream::out | std::ofstream::binary);

            if (file.good()) file.write(((const char*)this->heap), this->poolSize);
            file.close();
        }

        unsigned int MemoryPool::GetChunkSize() {
            return chunkSize;
        }

        unsigned int MemoryPool::GetPoolSize() {
            return poolSize;
        }

        unsigned int MemoryPool::GetUsedSize() {
            return usedSize;
        }

        unsigned int MemoryPool::GetFreeSize() {
            return poolSize - usedSize;
        }
    }
}
