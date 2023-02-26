#include "boidpch.h"
#include "GLSamplerState.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.inl"


namespace Polyboid
{

    static int32_t GetOpenglMinFilterMode(const MinFilterMode& filter)
    {
        switch (filter)
        {
            case MinFilterMode::Nearest: return GL_NEAREST;
            case MinFilterMode::Linear: return GL_LINEAR;
            case MinFilterMode::MipNearest: return GL_NEAREST_MIPMAP_NEAREST;
            case MinFilterMode::MipLinear: return  GL_LINEAR_MIPMAP_NEAREST;
            case MinFilterMode::Anisotropic: return -1;
        }

        return -1;
    }

    static int32_t GetOpenglMagFilterMode(const MagFilterMode& filter)
    {
        switch (filter)
        {
            case MagFilterMode::Nearest: return GL_NEAREST;
            case MagFilterMode::Linear: return GL_LINEAR;
        }
        return -1;
    }

    static int32_t GetOpenglTextureWrap(const TextureWrap& wrap)
    {
        switch (wrap)
        {
            case TextureWrap::Repeat: return GL_REPEAT;
            case TextureWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
            case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
            case TextureWrap::MirrorClampToEdge: return GL_MIRROR_CLAMP_TO_EDGE;
        }

        return -1;
    }

    static int32_t GetOpenGlTextureCompMode(const TextureCompareMode& compMode)
    {
        switch (compMode)
        {
        case TextureCompareMode::None: return GL_NONE;
        case TextureCompareMode::CompareRefToTexture: return GL_COMPARE_REF_TO_TEXTURE;
        }

        return -1;
    }

    int32_t GetTextureCompareFunc(const CompareFunction& compFunc)
    {
        switch (compFunc)
        {
        case CompareFunction::Never: return GL_NEVER;
        case CompareFunction::Less: return GL_LESS;
        case CompareFunction::Equal: return GL_EQUAL;
        case CompareFunction::LessEqual: return GL_LEQUAL;
        case CompareFunction::Greater: return GL_GREATER;
        case CompareFunction::NotEqual: return GL_NOTEQUAL;
        case CompareFunction::GreaterEqual: return GL_GEQUAL;
        case CompareFunction::Always: return GL_ALWAYS;
        }

        return -1;
    }
    
    GLSamplerState::GLSamplerState(const SamplerSettings& settings): m_Settings(settings)
    {

        glCreateSamplers(1, &m_Handle);
        glSamplerParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GetOpenglMinFilterMode(settings.minFilter));
        glSamplerParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, GetOpenglMagFilterMode(settings.magFilter));
        glSamplerParameteri(m_Handle, GL_TEXTURE_MIN_LOD, m_Settings.minLod);
        glSamplerParameteri(m_Handle, GL_TEXTURE_MAX_LOD, m_Settings.magLod);

        glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_S, GetOpenglTextureWrap(settings.wrap));
        glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_T, GetOpenglTextureWrap(settings.wrap));
        glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_R, GetOpenglTextureWrap(settings.wrap));

        glSamplerParameteri(m_Handle, GL_TEXTURE_COMPARE_MODE, GetOpenGlTextureCompMode(settings.compMode));
        glSamplerParameterfv(m_Handle, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(settings.borderColor));

        glSamplerParameteri(m_Handle, GL_TEXTURE_COMPARE_FUNC, GetTextureCompareFunc(settings.compFunction));

        
        
    }

    void GLSamplerState::SetFilterMode(const std::pair<MinFilterMode, MagFilterMode>& filter)
    {
        m_Settings.minFilter = filter.first;
        m_Settings.magFilter = filter.second;
    }

    std::pair<MinFilterMode, MagFilterMode> GLSamplerState::GetFilterMode()
    {
        return {m_Settings.minFilter, m_Settings.magFilter};
    }

    void GLSamplerState::SetWrapMode(const TextureWrap& wrap)
    {
        m_Settings.wrap = wrap;
    }

    TextureWrap GLSamplerState::GetWrapMode()
    {
        return m_Settings.wrap;
    }

    void GLSamplerState::SetCompareFunc(const CompareFunction& wrap)
    {
        m_Settings.compFunction = wrap;
    }

    CompareFunction GLSamplerState::GetCompareFunc()
    {
        return m_Settings.compFunction;
    }

    void GLSamplerState::SetBorderColor(const glm::vec4& borderColor)
    {
        m_Settings.borderColor = borderColor;
    }

    glm::vec4 GLSamplerState::GetBorderColor() const
    {
        return m_Settings.borderColor;
    }

    void GLSamplerState::Bind(uint32_t slot)
    {
        glBindSampler(m_Handle, slot);
    }

    void GLSamplerState::UnBind()
    {
        glBindSampler(0, 0);
    }

    GLSamplerState::~GLSamplerState()
    {
        glDeleteSamplers(1, &m_Handle);
    }
}
