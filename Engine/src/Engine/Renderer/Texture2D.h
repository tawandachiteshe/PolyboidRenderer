#pragma once
#include <any>

#include "Image2D.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{

    enum class TextureFilterMode
    {
        Nearest,
        Linear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear
    };

    enum class TextureWrapMode
    {
        Repeat = 0,
        ClampToEdge,
        MirroredRepeat,
        ClampToBorder
    };

    enum class TextureFormat
    {
        None = 0,
        RGBA,
        RGB,
        RG,
        R,
        Depth,
        DepthStencil,
        Stencil
    };

    enum class TextureDataType
    {
        None = 0,
        UnsignedByte,
        Byte,
        UnsignedShort,
        Short,
        UnsignedInt,
        Int,
        HalfFloat,
        Float,
        Double
    };


    enum class EngineGraphicsFormats
    {
        R8,
        R8UI,
        R8I,
        R16F,
        R16UI,
        R16I,
        R32F,
        R32UI,
        R32I,
        RG8,
        RG8UI,
        RG8I,
        RG16F,
        RG16UI,
        RG16I,
        RG32F,
        RG32UI,
        RG32I,
        RGB8,
        RGB8UI,
        RGB8I,
        RGB16F,
        RGB16UI,
        RGB16I,
        RGB32F,
        RGB32UI,
        RGB32I,
        RGBA8,
        BGRA8,
        RGBA8UI,
        RGBA8I,
        BGRA8I,
        BGRA8ISrgb,
        BGRA8U,
        RGBA16F,
        RGBA16UI,
        RGBA16I,
        RGBA32F,
        RGBA32UI,
        RGBA32I,
        DepthComponent16,
        DepthComponent32F,
        DepthComponent24,
        Depth24Stencil8,
        Depth32FStencil8
    };


    enum class TextureAccess
    {
        Read,
        Write,
        ReadWrite
    };

 
    
    struct TextureSettings
    {
        TextureFormat textureFormat = TextureFormat::RGBA;
        EngineGraphicsFormats sizedFormat = EngineGraphicsFormats::RGBA8;
        TextureDataType textureData = TextureDataType::UnsignedByte;
        TextureFilterMode filterMode = TextureFilterMode::Linear;
        TextureWrapMode wrapMode = TextureWrapMode::Repeat;
        bool generateMips = false;
        bool multiSample = false;

        ImageUsage usage = ImageUsage::Sampling;
        uint32_t sampleCount = 1;
        uint32_t mipCount = 1;

        uint32_t Width = 0;
        uint32_t Height = 0;
        uint32_t Depth = 0;

        std::string path;

    };


    struct TextureBufferSpecification
    {
        EngineGraphicsFormats sizedFormat = EngineGraphicsFormats::RGBA8;
        TextureAccess access = TextureAccess::ReadWrite;
        bool attachShaderStorage = false;
        uint32_t ssboBufferSlot = 0;
        
        uint32_t Width = 0;
        uint32_t Height = 0;
    };
    
    class Texture2D : public RenderResource
    {
    public:

        virtual void Bind(uint32_t slot = 0) = 0;
        virtual void UnBind() = 0;
        virtual std::any GetViewHandle() = 0;
        virtual std::any GetSamplerHandle() = 0;


        //Editor Only
        virtual std::any GetImGuiTexture() = 0;


        virtual void SetData(const void* data, uint32_t size) = 0;

        virtual ~Texture2D() = default;
        static Ref<Texture2D> Create(const TextureSettings& settings);
        static Ref<Texture2D> Create(const TextureSettings& settings, const void* data);
    };

    class Texture3D : public RenderResource
    {
    public:

        virtual void Bind(uint32_t slot = 0) = 0;
        virtual void UnBind() = 0;

        virtual void SetData(const void** data, uint32_t size) = 0;
        virtual std::any GetViewHandle() = 0;
        virtual std::any GetSamplerHandle() = 0;
        static Ref<Texture3D> Create(const TextureSettings& settings, const void* data);
        virtual ~Texture3D() = default;
    
    };
    
}

