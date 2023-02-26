#pragma once
#include "Engine/Renderer/BlendState.h"


namespace Polyboid
{
    class GLBlendState : public BlendState
    {
    private:
        std::vector<BlendMode> m_BlendModes;
        uint32_t m_SampleMask = 0xFFFF'FFFF;
        bool m_AlphaCoverage = false;
        bool m_RenderTargetBlend = false;
        glm::vec4 m_ConstColor = glm::vec4(1.0f);
        
    public:

        GLBlendState();
        
        void SetBlendMode(const BlendMode& blendMode) override;
        void SetBlendModes(const std::vector<Polyboid::BlendMode>& blendModes) override;

        const std::vector<BlendMode>& GetBlendModes() const override;
        void SetSampleMask(uint32_t sampleMask) override;

        uint32_t GetSampleMask() const override;
        void SetAlphaCoverage(bool enabled) override;

        bool GetAlphaCoverage() const override;
        void SetIndependentBlend(bool enabled) override;

        bool GetIndependentBlend() const override;

        void SetConstBlendFactor(const glm::vec4& constantBlendFactor) override;
        const glm::vec4& GetConstBlendFactor() const override;

        virtual void Bind();
        
        ~GLBlendState() override;
    };

}

