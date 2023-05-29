#include "boidpch.h"
#include "Allocators.h"



void* operator new(std::size_t size) {
    Polyboid::EngineMemoryManager::s_MemUsage += size;


    // Allocate memory with extra space to store the size
    std::size_t totalSize = size + sizeof(std::size_t);
    void* rawPtr = std::malloc(totalSize);

    // Store the size at the beginning of the allocated memory block
    std::memcpy(rawPtr, &size, sizeof(std::size_t));

    // Return the pointer to the memory block, excluding the size field
    return static_cast<char*>(rawPtr) + sizeof(std::size_t);

}

void operator delete(void* ptr) noexcept {
    void* originalPtr = static_cast<char*>(ptr) - sizeof(std::size_t);

    // Get the size from the stored field
    std::size_t size;
    std::memcpy(&size, originalPtr, sizeof(std::size_t));

    Polyboid::EngineMemoryManager::s_MemUsage -= size;


    // Free the memory block
    std::free(originalPtr);
}

namespace Polyboid
{
    uint64_t EngineMemoryManager::s_MemUsage = 0u;

    uint64_t EngineMemoryManager::GetMemoryUsage()
    {
        return s_MemUsage;
    }

}

