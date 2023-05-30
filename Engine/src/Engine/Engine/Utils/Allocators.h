#pragma once


namespace Polyboid
{


    class EngineMemoryManager
    {

    private:

        template<size_t Align>
        static void Deallocate(void* ptr) noexcept
        {
            operator delete(ptr);
        }


    public:

        static void* operator new(std::size_t size);

        static void operator delete(void* ptr);

        template<typename T, typename... Args>
        static T* AllocateMem(Args&&... args) {
            void* rawPtr = operator new(sizeof(T));
            if (rawPtr == nullptr) {
                throw std::bad_alloc();
            }
            return new(rawPtr) T(std::forward<Args>(args)...);
        }

        template<typename Type>
        static void FreeMem(Type* ptr)
        {
            Deallocate<8>(ptr);
        }

        static uint64_t s_AllocationCount;
        static uint64_t s_FreeCount;
        static uint64_t s_MemUsage;
        static uint64_t GetMemoryUsage();
        static uint64_t GetAllocationCount();
        static uint64_t GetFreeCount();

    };

}


