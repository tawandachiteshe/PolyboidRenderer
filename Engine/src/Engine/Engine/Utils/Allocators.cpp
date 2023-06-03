#include "boidpch.h"
#include "Allocators.h"

#include <spdlog/spdlog.h>

void* operator new(size_t size)
{
    Polyboid::EngineMemoryManager::s_MemUsage += size;
    Polyboid::EngineMemoryManager::s_AllocationCount += 1;
    return std::malloc(size);
}

void operator delete(void* memory, size_t size)
{
    Polyboid::EngineMemoryManager::s_Freed += size;
    Polyboid::EngineMemoryManager::s_FreeCount += 1;
    Polyboid::EngineMemoryManager::s_MemUsage -= size;
    std::free(memory);
}



namespace Polyboid
{
    uint64_t EngineMemoryManager::s_MemUsage = 0u;
    uint64_t EngineMemoryManager::s_AllocationCount = 0u;
    uint64_t EngineMemoryManager::s_FreeCount = 0u;
    uint64_t EngineMemoryManager::s_Freed = 0u;

    struct MemInfo
    {
        uint64_t size = 0;
        uint64_t count = 0;
        uint64_t padding = 0;
    };

    void* EngineMemoryManager::operator new(std::size_t size) {

        if (size == 4)
        {
            __debugbreak();
        }

        
        void* rawPtr = ::operator new(size);

        return (rawPtr);

    }



    void EngineMemoryManager::operator delete(void* ptr) noexcept {

        Polyboid::EngineMemoryManager::s_FreeCount += 1;
        // Free the memory block
        ::operator delete(ptr);


    }

    uint64_t EngineMemoryManager::GetMemoryUsage()
    {
        return s_MemUsage;
    }

    uint64_t EngineMemoryManager::GetFreed()
    {
        return s_Freed;
    }

    uint64_t EngineMemoryManager::GetAllocationCount()
    {
        return s_AllocationCount;
    }

    uint64_t EngineMemoryManager::GetFreeCount()
    {
        return s_FreeCount;
    }
}

