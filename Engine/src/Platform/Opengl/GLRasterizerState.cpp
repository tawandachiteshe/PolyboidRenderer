#include "boidpch.h"
#include "GLRasterizerState.h"

#include "glad/glad.h"


namespace Polyboid
{

    GLenum GetOpenGlCullFaceMode(CullFaceMode cullMode)
    {
        switch (cullMode)
        {
        case CullFaceMode::None: return 0;
        case CullFaceMode::Front: return GL_FRONT;
        case CullFaceMode::Back:  return GL_BACK;
        case CullFaceMode::FrontAndBack:  return GL_FRONT_AND_BACK;
        }
        
    }

    GLenum GetOpenGlFrontFaceDirection(FrontFaceDirection direction)
    {
	    switch (direction)
	    {
	    case FrontFaceDirection::CounterClockwise: return GL_CCW;
	    case FrontFaceDirection::Clockwise: return GL_CW;
	    }

        return 0;
    }

    GLenum GetOpenGlFillMode(FillMode mode)
    {
        switch (mode)
        {
        case FillMode::Wireframe: return GL_LINE;
        case FillMode::Solid: return GL_FILL;
        case FillMode::Point: return GL_POINT;
        }
    }
    
    GLRasterizerState::GLRasterizerState()
    {
        m_Viewports.resize(8, Viewport());
        m_ScissorRects.resize(8, Rect());
    }

    void GLRasterizerState::SetFillMode(FillMode frontFace, FillMode backFace)
    {
        m_FrontFaceFillMode = frontFace;
        m_BackFaceFillMode = backFace;
        m_StateDirty = true;
    }

    void GLRasterizerState::GetFillMode(FillMode& frontFace, FillMode& backFace) const
    {
        frontFace = m_FrontFaceFillMode;
        backFace = m_BackFaceFillMode;
    }

    void GLRasterizerState::SetCullMode(CullFaceMode cullMode)
    {
        m_CullMode = cullMode;
        m_StateDirty = true;
    }

    CullFaceMode GLRasterizerState::GetCullMode() const
    {
        return m_CullMode;
    }

    void GLRasterizerState::SetFrontFacing(FrontFaceDirection frontFace)
    {
        m_FrontFace = frontFace;
        m_StateDirty = true;
    }

    FrontFaceDirection GLRasterizerState::GetFrontFacing() const
    {
        return m_FrontFace;
    }

    void GLRasterizerState::SetDepthBias(const DepthBias& bias)
    {
        m_DepthBias = bias;
    }

    DepthBias GLRasterizerState::GetDepthBias() const
    {
        return m_DepthBias;
    }

    void GLRasterizerState::SetDepthClipEnabled(bool depthClipEnabled)
    {
        m_DepthClipEnabled = depthClipEnabled;
        m_StateDirty = true;
    }

    bool GLRasterizerState::GetDepthClipEnabled() const
    {
        return m_DepthClipEnabled;
    }

    void GLRasterizerState::SetScissorEnabled(bool scissorEnable)
    {
        m_ScissorEnabled = scissorEnable;
        m_StateDirty = true;
    }

    bool GLRasterizerState::GetScissorEnabled() const
    {
        return m_ScissorEnabled;
    }

    void GLRasterizerState::SetScissorRect(const Rect& rect)
    {
        m_ScissorRects[0] = rect;
        m_ScissorRectsDirty = true;
    }

    void GLRasterizerState::SetScissorRects(const std::vector<Rect>& rects)
    {
        m_ScissorRects = rects;
        m_ScissorRectsDirty = true;
    }

    const std::vector<Rect>& GLRasterizerState::GetScissorRects() const
    {
        return m_ScissorRects;
    }

    void GLRasterizerState::SetViewport(const Viewport& viewport)
    {
        m_Viewports[0] = viewport;
        m_ViewportsDirty = true;
    }

    void GLRasterizerState::SetViewports(const std::vector<Viewport>& viewports)
    {
        m_Viewports = viewports;
        m_ViewportsDirty = true;
    }

    const std::vector<Viewport>& GLRasterizerState::GetViewports()
    {
        return m_Viewports;
    }

    void GLRasterizerState::SetLineWidth(float size)
    {
        m_LineWidth = size;
        m_StateDirty = true;
    }

    float GLRasterizerState::GetLineWidth()
    {
        return m_LineWidth;
    }

    void GLRasterizerState::SetPointSize(float size)
    {
        m_PointSize = size;
        m_StateDirty = true;
    }

    float GLRasterizerState::GetPointSize()
    {
        return m_PointSize;
    }

    void GLRasterizerState::Bind()
    {
        if (m_StateDirty)
        {
            if (static_cast<uint32_t>(m_CullMode))
            {
                glDisable(GL_CULL_FACE);
            }
            else
            {
                glEnable(GL_CULL_FACE);
                glCullFace(GetOpenGlCullFaceMode(m_CullMode));
            }
        
            glPolygonMode(GL_FRONT, GetOpenGlFillMode(m_FrontFaceFillMode));
            glPolygonMode(GL_BACK, GetOpenGlFillMode(m_BackFaceFillMode));

            glLineWidth(m_LineWidth);
            glPointSize(m_PointSize);

            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffsetClamp(m_DepthBias.depthBias, m_DepthBias.slopeBias, m_DepthBias.biasClamp);

            glFrontFace(GetOpenGlFrontFaceDirection(m_FrontFace));

            if(m_DepthClipEnabled)
            {
                glEnable(GL_DEPTH_CLAMP);
            }
            else
            {
                glDisable(GL_DEPTH_CLAMP);
            }

            if (m_ScissorEnabled)
            {
                glEnable(GL_SCISSOR_TEST);
            }
            else
            {
                glDisable(GL_SCISSOR_TEST);
            }

            if (m_AntialiasedLineEnabled)
            {
                glEnable(GL_LINE_SMOOTH);
            }
            else
            {
                glDisable(GL_LINE_SMOOTH); 
            }
            
            if (m_MultisampleEnabled)
            {
                glEnable(GL_MULTISAMPLE); 
            }
            else
            {
                glDisable(GL_MULTISAMPLE); 
            }

            m_StateDirty = false;
        }


        if (m_ViewportsDirty)
        {
            uint32_t count = 0;
            for (auto& viewPort : m_Viewports)
            {
                glViewportIndexedf(count, viewPort.X, viewPort.Y, viewPort.Width, viewPort.Height);
                glDepthRangeIndexed(count, (GLdouble)viewPort.MinDepth, (GLdouble)viewPort.MaxDepth);
                count++;
            }
            
            m_ViewportsDirty = false;
        }


        if (m_ScissorRectsDirty)
        {

            uint32_t count = 0;

            for (auto& rect : m_ScissorRects)
            {
                glScissorIndexed(count, rect.X, rect.Y, rect.Width, rect.Height);
                count++;
            }
            
            m_ScissorRectsDirty = false;
        }
        
    }
}
