#pragma once

#include "Exceptions.hpp"

#include "Statistics.hpp"


namespace {

	template<typename T = void*>
	struct _MemoryAllocBlock {
		size_t AllocationBytes;
		T Allocation[];
	};


    static void* _Allocate(size_t sz) {
        unlikely_branch
        if (sz == 0) {
           Exceptions::ThrowException<Exceptions::BadAllocation>("Attempted to allocate a block of memory with a size of 0.");
        }

        if (void* const ptr = std::malloc(sz + sizeof(size_t))) {
            using namespace Game::Statistics::Memory;
            EngineAllocatedBytes += sz;
            EngineAllocationBytesPerSecond += sz;
            ++EngineAllocations;
            ++EngineAllocationsPerSecond;

            *reinterpret_cast<size_t*>(ptr) = sz;

            return ptr + sizeof(size_t);
        }

        Exceptions::ThrowException<Exceptions::BadAllocation>();
    }

    static void _Free(void* ptr) {
        using namespace Game::Statistics::Memory;

        ::_MemoryAllocBlock<>* MemBlock = std::move(reinterpret_cast<::_MemoryAllocBlock<>*>(ptr));
        MemBlock -= sizeof(size_t);

        EngineAllocatedBytes -= MemBlock->AllocationBytes;
        ++EngineDeallocationsPerSecond;

        std::free(reinterpret_cast<void*>(MemBlock));
    }
}

// no inline, required by [replacement.functions]/3
void* operator new(size_t sz) {
    return ::_Allocate(sz);
}

// no inline, required by [replacement.functions]/3
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