#pragma once
#include "Engine/Renderer/DepthStencilState.h"


namespace Polyboid
{
    class GLDepthStencilState : public DepthStencilState
    {
    private:
        DepthMode m_DepthMode;
        StencilMode m_StencilMode;

    
        
    public:
        void SetDepthMode(const DepthMode& depthMode) override;
        const DepthMode& GetDepthMode() const override;
        void SetStencilMode(const StencilMode& stencilMode) override;
        const StencilMode& GetStencilMode() const override;
        ~GLDepthStencilState() override;

        virtual void Bind();
    };

}

