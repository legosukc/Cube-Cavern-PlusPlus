#pragma once

#include "define.h"

#include <SDL3/SDL_timer.h>


#include "Statistics.hpp"

#include "FunctionHeaders/Exceptions.hpp"


namespace {

	template<typename T = void*>
	struct _MemoryAllocBlock {
		size_t AllocationBytes;
		T Allocation[];
	};

    static void* _Allocate(size_t sz) {

        const Uint64 StartNS = SDL_GetTicksNS();

        unlikely_branch
        if (sz == 0) {
           Exceptions::ThrowException<Exceptions::RangeError>("Attempted to allocate a block of memory with a size of 0.");
        }

        likely_branch
        if (size_t* const ptr = static_cast<size_t*>(std::malloc(sz + sizeof(size_t)))) {

            Game::Statistics::Memory::EngineAllocatedBytes += sz;
            Game::Statistics::Memory::EngineAllocationBytesPerFrame += sz;
            ++Game::Statistics::Memory::EngineAllocations;
            ++Game::Statistics::Memory::EngineAllocationsPerFrame;

            *ptr = sz;

            Game::Statistics::Memory::EngineNSSpentOnHeapPerFrame += SDL_GetTicksNS() - StartNS;

            return reinterpret_cast<void*>(ptr + sizeof(size_t));
        }

        Exceptions::ThrowException<Exceptions::RuntimeError>("Memory allocation exception.");
    }

    static void _Free(void* ptr) {

        const Uint64 StartNS = SDL_GetTicksNS();

        ::_MemoryAllocBlock<>* MemBlock = static_cast<::_MemoryAllocBlock<>*>(ptr) - sizeof(size_t);

        Game::Statistics::Memory::EngineAllocatedBytes -= MemBlock->AllocationBytes;
        ++Game::Statistics::Memory::EngineDeallocationsPerFrame;

        std::free(reinterpret_cast<void*>(MemBlock));

        Game::Statistics::Memory::EngineNSSpentOnHeapPerFrame += SDL_GetTicksNS() - StartNS;
    }
}

/*
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
}*/

// TODO: do
namespace Memory {

    void* Reallocate(void* ptr, size_t NewSize) {
        
        return NULL;
    }
}