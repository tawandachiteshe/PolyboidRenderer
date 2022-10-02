#pragma once
#include <cstdint>
#include <memory>

namespace Polyboid
{
    class IndexBuffer
    {

    private:
        uint32_t m_ID = 0;
        uint32_t m_Count = 0;
    
        
    public:
        void Bind();
        IndexBuffer(const uint32_t* indices, uint32_t count);
        ~IndexBuffer();
        uint32_t GetCount() { return m_Count; }

        static std::shared_ptr<IndexBuffer> MakeIndexBuffer(const uint32_t* indices, uint32_t count);
    
    };

}

