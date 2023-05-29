#pragma once


namespace Polyboid
{
    void* ::operator new(std::size_t size);

	void ::operator delete(void* ptr);

    template<typename T, typename... Args>
    T* AllocateMem(Args&&... args) {
        void* rawPtr = operator new(sizeof(T));
        if (rawPtr == nullptr) {
            throw std::bad_alloc();
        }
        return new(rawPtr) T(std::forward<Args>(args)...);
    }

    template<typename T>
    void FreeMem(T* ptr)
	{
        operator delete(ptr);
	}

    class EngineMemoryManager
    {
    public:

        static uint64_t s_MemUsage;
        static uint64_t GetMemoryUsage();

    };

}


