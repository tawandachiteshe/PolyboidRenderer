#pragma once


namespace Polyboid
{
    enum class DepthFunction
    {
        Never = 0,
        Less,
        Equal,
        LessThanOrEqual,
        Greater,
        NotEqual,
        GreaterThanOrEqual,
        Always
    };
    
    enum class StencilFunction
    {
        Never = 0,
        Less,
        Equal,
        LessThanOrEqual,
        Greater,
        NotEqual,
        GreaterThanOrEqual,
        Always
    };

    enum class StencilOp
    {
        Keep = 0,
        Zero,
        Replace,
        Increment,
        IncrementWrap,
        Decrement,
        DecrementWrap,
        Invert
    };

    enum class DepthMask
    {
        Enabled = 0,
        Disabled
    };

    enum class StencilMask
    {
        All = 0
    };

 enum class DepthWrite
 {
  Enable,
  Disable
};


     struct DepthMode
    {
        bool DepthEnable = false;
         
        DepthWrite DepthWriteMask = DepthWrite::Enable;
        DepthFunction depthFunction = DepthFunction::Less;
      
    };


    struct FaceOperation
    {
        StencilOp StencilFail = StencilOp::Keep;
        StencilOp StencilPassDepthFail = StencilOp::Keep;
        StencilOp StencilDepthPass = StencilOp::Keep;
        StencilFunction stencilFunction = StencilFunction::LessThanOrEqual;
    };

    struct StencilMode
    {
        /**
         * Set to true to enable stencil testing.
         * Default: false.
         */
        bool StencilEnabled = false;

        /**
         * A mask that is AND'd to the value in the stencil buffer before it is read.
         * Default: 0xff
         */
        uint8_t ReadMask = 0xFF;

        /**
         * A mask that is AND'd to the value in the stencil buffer before it is written.
         * Default: 0xff
         */
        uint8_t WriteMask = 0xFF;

        /**
         * The value to set the stencil buffer to if any of the StencilOperation 
         * members of the FaceOperation struct is set to StencilOperation::Reference.
         * Default: 0
         */
        int32_t StencilReference = 0;

        /**
         * The compare function and pass/fail operations to perform on the stencil
         * buffer for front-facing polygons.
         */
        FaceOperation FrontFace;

        /**
         * The compare function and pass/fail operations to perform on the stencil 
         * buffer for back-facing polygons.
         */
        FaceOperation BackFace;
     
    };
    
    class DepthStencilState
    {
    public:
        virtual void SetDepthMode( const DepthMode& depthMode ) = 0;
        virtual const DepthMode& GetDepthMode() const = 0;

        virtual void SetStencilMode( const StencilMode& stencilMode ) = 0;
        virtual const StencilMode& GetStencilMode() const = 0;

        virtual ~DepthStencilState() = default;
    };
}
