#pragma once
#include "Engine/Renderer/Renderbuffer.h"


namespace Polyboid
{
    class GLRenderbuffer : public Renderbuffer
    {

    private:
        uint32_t m_Handle = 0;
        RenderbufferSettings m_Settings;
        
    public:

        GLRenderbuffer(const RenderbufferSettings& settings);
        
        RenderbufferSettings GetSettings() override;
        ~GLRenderbuffer() override;

        friend class GLFramebuffer;
    };
}

