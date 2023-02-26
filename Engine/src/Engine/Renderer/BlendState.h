#pragma once
#include "glm/vec4.hpp"


namespace Polyboid
{
    enum class BlendFactor
    {
        Zero = 0,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        ConstantColor,
        OneMinusConstantColor,
        ConstantAlpha,
        OneMinusConstantAlpha,
        SrcAlphaSaturate,
        Src1Color,
        OneMinusSrc1Color,
        Src1Alpha,
        OneMinusSrc1Alpha
    };

    enum class BlendOperation
    {
        Add = 0,
        Subtract,
        ReverseSubtract,
        Min,
        Max
    };


    enum class BlendLogicOperation
    {
        Clear = 0,
        Set,
        Copy,
        CopyInverted,
        Noop,
        Invert,
        And,
        Nand,
        Or,
        Nor,
        Xor,
        Equiv,
        AndReverse,
        AndInverted,
        OrReverse,
        OrInverted
    };

    struct BlendMode
    {
        bool BlendEnabled = false;

        bool LogicOpEnabled = false;
        BlendFactor SrcFactor = BlendFactor::One;
        BlendFactor DstFactor = BlendFactor::Zero;
        BlendOperation BlendOp = BlendOperation::Add;

        BlendFactor SrcAlphaFactor = BlendFactor::Zero;
        BlendFactor DstAlphaFactor = BlendFactor::Zero;
        BlendOperation AlphaOp = BlendOperation::Add;
        BlendLogicOperation LogicOp = BlendLogicOperation::Noop;

        bool WriteRed = true;
        bool WriteGreen = true;
        bool WriteBlue = true;
        bool WriteAlpha = true;
    };

    class BlendState
    {
    public:
        
        virtual void SetBlendMode(const BlendMode& blendMode) = 0;
        virtual void SetBlendModes(const std::vector<BlendMode>& blendModes) = 0;
        virtual const std::vector<BlendMode>& GetBlendModes() const = 0;

        virtual void SetSampleMask(uint32_t sampleMask) = 0;
        virtual uint32_t GetSampleMask() const = 0;

        virtual void SetAlphaCoverage(bool enabled) = 0;
        virtual bool GetAlphaCoverage() const = 0;

        virtual void SetIndependentBlend(bool enabled) = 0;
        virtual bool GetIndependentBlend() const = 0;

        virtual void SetConstBlendFactor(const glm::vec4& constantBlendFactor) = 0;
        virtual const glm::vec4& GetConstBlendFactor() const = 0;

        virtual ~BlendState() = default;
        
    };
}
