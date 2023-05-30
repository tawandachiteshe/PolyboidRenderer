#include "boidpch.h"
#include "Allocators.h"


struct MemInfo
{
    uint64_t size = 0;
    uint64_t count = 0;
};

void* operator new(std::size_t size) {
    Polyboid::EngineMemoryManager::s_MemUsage += size;
    Polyboid::EngineMemoryManager::s_AllocationCount += 1;

    MemInfo memInfo{};
    memInfo.size = size;
    memInfo.count += 1;


    // Allocate memory with extra space to store the size
    std::size_t totalSize = size + sizeof(MemInfo);
    void* rawPtr = std::malloc(totalSize);

    // Store the size at the beginning of the allocated memory block
    std::memcpy(rawPtr, &memInfo, sizeof(MemInfo));

    // Return the pointer to the memory block, excluding the size field
    return static_cast<char*>(rawPtr) + sizeof(MemInfo);

}

void operator delete(void* ptr) noexcept {

	if (ptr)
	{
       
        void* originalPtr = static_cast<char*>(ptr) - sizeof(MemInfo);

        // Get the size from the stored field
        MemInfo memInfo{};
        std::memcpy(&memInfo, originalPtr, sizeof(MemInfo));

        Polyboid::EngineMemoryManager::s_MemUsage -= memInfo.size;
        Polyboid::EngineMemoryManager::s_FreeCount += memInfo.count;


        // Free the memory block
        std::free(originalPtr);
	}
	else
	{
        __debugbreak();
	}

}

namespace Polyboid
{
    uint64_t EngineMemoryManager::s_MemUsage = 0u;
    uint64_t EngineMemoryManager::s_AllocationCount = 0u;
    uint64_t EngineMemoryManager::s_FreeCount = 0u;

    uint64_t EngineMemoryManager::GetMemoryUsage()
    {
        return s_MemUsage;
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

