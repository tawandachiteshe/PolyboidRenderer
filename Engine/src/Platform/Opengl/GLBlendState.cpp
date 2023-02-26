#include "boidpch.h"
#include "GLBlendState.h"

#include <glad/glad.h>


namespace Polyboid
{
    GLenum GetOpenglBlendOp(const BlendOperation& op)
    {
        switch (op)
        {
        case BlendOperation::Add: return GL_FUNC_ADD;
        case BlendOperation::Subtract: return GL_FUNC_SUBTRACT;
        case BlendOperation::ReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT;
        case BlendOperation::Min: return GL_MIN;
        case BlendOperation::Max: return GL_MAX;
        }
        return 0;
    }

    GLenum GetOpenglBlendLogicOp(const BlendLogicOperation& logicOp)
    {
        switch (logicOp)
        {
        case BlendLogicOperation::Clear: return GL_CLEAR;
        case BlendLogicOperation::Set: return GL_SET;
        case BlendLogicOperation::Copy: return GL_COPY;
        case BlendLogicOperation::CopyInverted: return GL_COPY_INVERTED;
        case BlendLogicOperation::Noop: return GL_NOOP;
        case BlendLogicOperation::Invert: return GL_INVERT;
        case BlendLogicOperation::And: return GL_AND;
        case BlendLogicOperation::Nand: return GL_NAND;
        case BlendLogicOperation::Or: return GL_OR;
        case BlendLogicOperation::Nor: return GL_NOR;
        case BlendLogicOperation::Xor: return GL_XOR;
        case BlendLogicOperation::Equiv: return GL_EQUIV;
        case BlendLogicOperation::AndReverse: return GL_AND_REVERSE;
        case BlendLogicOperation::AndInverted: return GL_AND_INVERTED;
        case BlendLogicOperation::OrReverse: return GL_OR_REVERSE;
        case BlendLogicOperation::OrInverted: return GL_OR_INVERTED;
        }

        return 0;
    }

    GLenum GetOpenglBlendFactor(const BlendFactor& factor)
    {
        switch (factor)
        {
        case BlendFactor::Zero: return GL_ZERO;
        case BlendFactor::One: return GL_ONE;
        case BlendFactor::SrcColor: return GL_SRC_COLOR;
        case BlendFactor::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
        case BlendFactor::DstColor: return GL_DST_COLOR;
        case BlendFactor::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
        case BlendFactor::SrcAlpha: return GL_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DstAlpha: return GL_DST_ALPHA;
        case BlendFactor::OneMinusDstAlpha: return GL_ZERO;
        case BlendFactor::ConstantColor: return GL_CONSTANT_COLOR;
        case BlendFactor::OneMinusConstantColor: return GL_ONE_MINUS_CONSTANT_COLOR;
        case BlendFactor::ConstantAlpha: return GL_CONSTANT_ALPHA;
        case BlendFactor::OneMinusConstantAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
        case BlendFactor::SrcAlphaSaturate: return GL_SRC_ALPHA_SATURATE;
        case BlendFactor::Src1Color: return GL_SRC1_COLOR;
        case BlendFactor::OneMinusSrc1Color: return GL_ONE_MINUS_SRC_COLOR;
        case BlendFactor::Src1Alpha: return GL_SRC1_ALPHA;
        case BlendFactor::OneMinusSrc1Alpha: return GL_ONE_MINUS_SRC_ALPHA;
        }

        return 0;
    }

    GLBlendState::GLBlendState()
    {
        m_BlendModes.resize(8);
    }

    void GLBlendState::SetBlendMode(const Polyboid::BlendMode& blendMode)
    {
        m_BlendModes[0] = blendMode;
    }

    void GLBlendState::SetBlendModes(const std::vector<Polyboid::BlendMode>& blendModes)
    {
        m_BlendModes = blendModes;
    }

    const std::vector<Polyboid::BlendMode>& GLBlendState::GetBlendModes() const
    {
        return m_BlendModes;
    }

    void GLBlendState::SetSampleMask(uint32_t sampleMask)
    {
        m_SampleMask = sampleMask;
    }

    uint32_t GLBlendState::GetSampleMask() const
    {
        return m_SampleMask;
    }

    void GLBlendState::SetAlphaCoverage(bool enabled)
    {
        
    }

    bool GLBlendState::GetAlphaCoverage() const
    {
        return true;
    }

    void GLBlendState::SetIndependentBlend(bool enabled)
    {
        
    }

    bool GLBlendState::GetIndependentBlend() const
    {
        return true;
    }

    void GLBlendState::SetConstBlendFactor(const glm::vec4& constantBlendFactor)
    {
        m_ConstColor = constantBlendFactor;
    }

    const glm::vec4& GLBlendState::GetConstBlendFactor() const
    {
        return m_ConstColor;
    }

    void GLBlendState::Bind()
    {
        for (int i = 0; i < 8 && i < m_BlendModes.size(); ++i)
        {
            glEnablei(GL_BLEND, i);
            BlendMode& mode = m_BlendModes[i];
           const uint32_t srcAlpha = GetOpenglBlendFactor(mode.SrcAlphaFactor);
           const uint32_t dstAlpha = GetOpenglBlendFactor(mode.DstAlphaFactor);

           const uint32_t srcColor = GetOpenglBlendFactor(mode.SrcFactor);
           const uint32_t dstColor = GetOpenglBlendFactor(mode.DstFactor);

           const uint32_t colorOp = GetOpenglBlendOp(mode.BlendOp);
           const uint32_t alphaOp = GetOpenglBlendOp(mode.AlphaOp);
           const uint32_t logicOp = GetOpenglBlendLogicOp(mode.LogicOp);

            //Color Blend
            glBlendEquationSeparatei(i, colorOp, alphaOp);
            glBlendFuncSeparatei(i, srcColor, dstColor, srcAlpha, dstAlpha);
            glBlendColor(m_ConstColor.x, m_ConstColor.y, m_ConstColor.z, m_ConstColor.z);
            glLogicOp(logicOp);
            glColorMaski(i, mode.WriteRed, mode.WriteGreen, mode.WriteBlue, mode.WriteAlpha);

            glSampleMaski(0, m_SampleMask);
        }
    }

    GLBlendState::~GLBlendState()
    {
    }
}
