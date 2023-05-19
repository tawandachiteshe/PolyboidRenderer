#pragma once


#include "Shader.h"
#include "Texture.h"


namespace Polyboid
{
	enum class IndexDataType;

    class StorageBuffer
    {
    public:
        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void Unbind() const = 0;
        virtual void SetData(const void* data, uint32_t sizeData, uint32_t offset = 0) = 0;
        virtual std::any GetHandle() = 0;
        virtual ~StorageBuffer() = default;
        static Ref<StorageBuffer> Create(uint32_t size);
    };


    
    class TextureBuffer
    {
    public:
        virtual void Bind(uint32_t slot = 0, uint32_t bufferSlot = 0) const = 0;
        virtual void Unbind() const = 0;
        virtual ~TextureBuffer() = default;
    };
    

    struct BufferElement
    {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        size_t Offset;
        bool Normalized;

        BufferElement() = default;

        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
        {
        }

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
            case ShaderDataType::Float:   return 1;
            case ShaderDataType::Float2:  return 2;
            case ShaderDataType::Float3:  return 3;
            case ShaderDataType::Float4:  return 4;
            case ShaderDataType::Mat3:    return 3; // 3* float3
                case ShaderDataType::Mat4:    return 4; // 4* float4
                case ShaderDataType::Int:     return 1;
            case ShaderDataType::Int2:    return 2;
            case ShaderDataType::Int3:    return 3;
            case ShaderDataType::Int4:    return 4;
            case ShaderDataType::Bool:    return 1;
            }
            
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() {}

        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        }

        uint32_t GetStride() const { return m_Stride; }
        const std::vector<BufferElement>& GetElements() const { return m_Elements; }

        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
    private:
        void CalculateOffsetsAndStride()
        {
            size_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }
    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
        
    };
    
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(const void* data, uint32_t size) = 0;

        virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;

        virtual uint32_t GetSizeInBytes() const = 0;
        virtual std::any GetHandle() const = 0;

        static Ref<VertexBuffer> Create(const void* data, uint32_t size);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual uint32_t GetSizeInBytes() const = 0;
        virtual std::any GetHandle() const = 0;

        virtual IndexDataType GetIndexDataType() = 0;
        virtual uint32_t GetCount() const = 0;

        static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
    };

    class StagingBuffer
    {
    public:
        virtual void SetData(const void* data) = 0;
        virtual uint32_t GetSizeInBytes() = 0;
        virtual std::any GetHandle() = 0;
        virtual ~StagingBuffer() = default;

    };

}
