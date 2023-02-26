#include "boidpch.h"
#include "GLDepthStencilState.h"
#include <glad/glad.h>


namespace Polyboid
{

    bool GetOpenGlWriteMask(const DepthWrite& write)
    {
        switch (write)
        {
        case DepthWrite::Enable: return GL_TRUE;
        case DepthWrite::Disable: return GL_FALSE;
        }

        return GL_FALSE;
        
    }

    GLenum GetOpenglDepthCompFunc(const DepthFunction& depthFunction)
    {
        switch (depthFunction)
        {
        case DepthFunction::Never: return GL_NEVER;
        case DepthFunction::Less: return GL_LESS;
        case DepthFunction::Equal: return GL_EQUAL;
        case DepthFunction::LessThanOrEqual: return GL_LEQUAL;
        case DepthFunction::Greater: return GL_GREATER;
        case DepthFunction::NotEqual: return GL_NOTEQUAL;
        case DepthFunction::GreaterThanOrEqual: return GL_GEQUAL;
        case DepthFunction::Always:return GL_ALWAYS;
        }

        return 0;
    }

    GLenum GetOpenglStencilCompFunc(const StencilFunction& stencilFunction)
    {
        switch (stencilFunction)
        {
        case StencilFunction::Never: return GL_NEVER;
        case StencilFunction::Less: return GL_LESS;
        case StencilFunction::Equal: return GL_EQUAL;
        case StencilFunction::LessThanOrEqual: return GL_LEQUAL;
        case StencilFunction::Greater: return GL_GREATER;
        case StencilFunction::NotEqual: return GL_NOTEQUAL;
        case StencilFunction::GreaterThanOrEqual: return GL_GEQUAL;
        case StencilFunction::Always:return GL_ALWAYS;
        }

        return 0;
    }

    GLenum GetOpenglStencilOp(const StencilOp& stencilOp)
    {
        switch (stencilOp)
        {
        case StencilOp::Keep: return GL_KEEP;
        case StencilOp::Zero: return GL_ZERO;
        case StencilOp::Replace: return GL_REPLACE;
        case StencilOp::Increment: return GL_INCR;
        case StencilOp::IncrementWrap: return GL_INCR_WRAP;
        case StencilOp::Decrement: return GL_DECR;
        case StencilOp::DecrementWrap: return GL_DECR_WRAP;
        case StencilOp::Invert: return GL_INVERT;
        }

        return 0;
    }
    
    void GLDepthStencilState::SetDepthMode(const DepthMode& depthMode)
    {
        m_DepthMode = depthMode;
    }

    const DepthMode& GLDepthStencilState::GetDepthMode() const
    {
        return m_DepthMode;
    }

    void GLDepthStencilState::SetStencilMode(const StencilMode& stencilMode)
    {
        m_StencilMode = stencilMode;
    }

    const StencilMode& GLDepthStencilState::GetStencilMode() const
    {
        return m_StencilMode;
    }

    GLDepthStencilState::~GLDepthStencilState()
    {
        
    }

    void GLDepthStencilState::Bind()
    {
        //Depth stuff hehre my guy
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GetOpenGlWriteMask(m_DepthMode.DepthWriteMask));
        glDepthFunc(GetOpenglDepthCompFunc(m_DepthMode.depthFunction));

        //Stencil stuff hehrer
        glEnable(GL_STENCIL_TEST);
        glStencilMask(m_StencilMode.WriteMask);

       //Stencil front and back
        //glStencilFuncSeparate(GL_FRONT, m_StencilMode.
        FaceOperation& frontFaceOp = m_StencilMode.FrontFace;
        FaceOperation& backFaceOp = m_StencilMode.BackFace;

        //Front
        uint32_t fsfail = GetOpenglStencilOp(frontFaceOp.StencilFail);
        uint32_t fdpfail = GetOpenglStencilOp(frontFaceOp.StencilPassDepthFail);
        uint32_t fdppass = GetOpenglStencilOp(frontFaceOp.StencilDepthPass);
        glStencilOpSeparate(GL_FRONT, fsfail, fdpfail, fdppass);
        glStencilFuncSeparate(GL_FRONT, GetOpenglStencilCompFunc(frontFaceOp.stencilFunction), m_StencilMode.StencilReference, m_StencilMode.WriteMask);

        //Back
        uint32_t bsfail = GetOpenglStencilOp(backFaceOp.StencilFail);
        uint32_t bdpfail = GetOpenglStencilOp(backFaceOp.StencilPassDepthFail);
        uint32_t bdppass = GetOpenglStencilOp(backFaceOp.StencilDepthPass);
        glStencilOpSeparate(GL_BACK, bsfail, bdpfail, bdppass);
        glStencilFuncSeparate(GL_BACK, GetOpenglStencilCompFunc(backFaceOp.stencilFunction), m_StencilMode.StencilReference, m_StencilMode.WriteMask);
        
    }
}
