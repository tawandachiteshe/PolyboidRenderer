#pragma once
#include <any>

#include "Engine/Engine/Base.h"
#include "glm/vec4.hpp"


namespace Polyboid
{

    enum class MinFilterMode
    {
        Nearest,
        Linear,
        MipNearest,
        MipLinear,
        Anisotropic
    };

    
    enum class MagFilterMode
    {
        Nearest,
        Linear,
    };

    enum class TextureWrap
    {
        Repeat,
        ClampToEdge,
        MirroredRepeat,
        MirrorClampToEdge,
        ClampToBorder
    };

    enum class CompareFunction
    {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always
    };
    
    
    enum class TextureCompareMode
    {
        None,
        CompareRefToTexture
    };
    

    struct SamplerSettings
    {
        MinFilterMode minFilter = MinFilterMode::Linear;
        MagFilterMode magFilter = MagFilterMode::Linear;
        MinFilterMode mipModeFilter = MinFilterMode::MipLinear;
        TextureWrap wrap = TextureWrap::ClampToEdge;
        CompareFunction compFunction = CompareFunction::Less;
        TextureCompareMode compMode = TextureCompareMode::None;
        glm::vec4 borderColor = glm::vec4(0.0f);
        int32_t minLod = -1000;
        int32_t magLod = 1000;
    };
    
    class SamplerState
    {
    public:

        //TODO: add anistrophic filtering

        virtual void SetFilterMode(const std::pair<MinFilterMode, MagFilterMode>& filter) = 0;
        virtual std::pair<MinFilterMode, MagFilterMode> GetFilterMode() = 0;
        
        virtual void SetWrapMode(const TextureWrap& wrap) = 0;
        virtual TextureWrap GetWrapMode() = 0;

        virtual void SetCompareFunc(const CompareFunction& wrap) = 0;
        virtual CompareFunction GetCompareFunc() = 0;

        virtual void SetBorderColor( const glm::vec4& borderColor ) = 0;
        virtual glm::vec4 GetBorderColor() const = 0;
        
        virtual void Bind(uint32_t slot) = 0;
        virtual void UnBind() = 0;

        virtual std::any GetSamplerHandle() = 0;

        virtual ~SamplerState() = default;

        static Ref<SamplerState> Create(const SamplerSettings& settings);
    
    };

}

