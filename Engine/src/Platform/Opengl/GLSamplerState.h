#pragma once
#include "Engine/Renderer/SamplerState.h"


namespace Polyboid
{
    class GLSamplerState : public SamplerState
    {
    private:
        uint32_t m_Handle = 0;
        SamplerSettings m_Settings;
    
    public:

        GLSamplerState(const SamplerSettings& settings);
        
        void SetFilterMode(const std::pair<MinFilterMode, MagFilterMode>& filter) override;
        std::pair<MinFilterMode, MagFilterMode> GetFilterMode() override;
        
        void SetWrapMode(const TextureWrap& wrap) override;
        TextureWrap GetWrapMode() override;

        void SetCompareFunc(const CompareFunction& wrap) override;
        CompareFunction GetCompareFunc() override;

        void SetBorderColor(const glm::vec4& borderColor) override;
        glm::vec4 GetBorderColor() const override;

        void Bind(uint32_t slot) override;
        void UnBind() override;


        ~GLSamplerState() override;
    };
}


