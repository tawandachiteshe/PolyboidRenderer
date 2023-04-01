#pragma once
#include "Texture.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
    struct RenderbufferSettings
    {
        EngineGraphicsFormats format = EngineGraphicsFormats::RGBA8;
        uint32_t width = 0;
        uint32_t height = 0;
    };
    
    class Renderbuffer
    {
    public:
        virtual RenderbufferSettings GetSettings() = 0;
        virtual ~Renderbuffer() = default;
    };
}
