#pragma once

#include "define.h"

#include <chrono>


#include "Statistics.hpp"

#include "FunctionHeaders/Exceptions.hpp"


namespace {

	template<typename T = void*>
	struct _MemoryAllocBlock {
		size_t AllocationBytes;
		T Allocation[];
	};


    static thread_local std::chrono::high_resolution_clock::time_point _StartTime;

    static void* _Allocate(size_t sz) {

        ::_StartTime = std::chrono::high_resolution_clock::now();

        unlikely_branch
        if (sz == 0) {
           Exceptions::ThrowException<Exceptions::BadAllocation>("Attempted to allocate a block of memory with a size of 0.");
        }

        likely_branch
        if (size_t* const ptr = static_cast<size_t*>(std::malloc(sz + sizeof(size_t)))) {

            Game::Statistics::Memory::EngineAllocatedBytes += sz;
            Game::Statistics::Memory::EngineAllocationBytesPerFrame += sz;
            ++Game::Statistics::Memory::EngineAllocations;
            ++Game::Statistics::Memory::EngineAllocationsPerFrame;

            *ptr = sz;

            Game::Statistics::Memory::EngineMicrosecondsSpentOnHeapPerFrame += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ::_StartTime).count();

            return reinterpret_cast<void*>(ptr + sizeof(size_t));
        }

        Exceptions::ThrowException<Exceptions::BadAllocation>();
    }

    static void _Free(void* ptr) {

        ::_StartTime = std::chrono::high_resolution_clock::now();

        ::_MemoryAllocBlock<>* MemBlock = std::move(static_cast<::_MemoryAllocBlock<>*>(ptr));
        MemBlock -= sizeof(size_t);

        std::free(reinterpret_cast<void*>(MemBlock));

        Game::Statistics::Memory::EngineAllocatedBytes -= MemBlock->AllocationBytes;
        ++Game::Statistics::Memory::EngineDeallocationsPerFrame;

        Game::Statistics::Memory::EngineMicrosecondsSpentOnHeapPerFrame += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ::_StartTime).count();
    }
}


void* operator new(size_t sz) {
    return ::_Allocate(sz);
}
void* operator new[](size_t sz) {
    return ::_Allocate(sz);
}

void operator delete(void* ptr) noexcept {
    ::_Free(ptr);
}
void operator delete(void* ptr, size_t) noexcept {
    ::_Free(ptr);
}
void operator delete[](void* ptr) noexcept {
    ::_Free(ptr);
}
void operator delete[](void* ptr, size_t) noexcept {
    ::_Free(ptr);
}


namespace Memory {

    void* Reallocate(void* ptr, size_t NewSize) {

    }
}