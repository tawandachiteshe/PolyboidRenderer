#pragma once
#include "Rect.h"
#include "Viewport.h"


namespace Polyboid
{

    enum class CullFaceMode
    {
        None = 0,
        Front,
        Back,
        FrontAndBack
    };

    enum class PolygonMode
    {
        Fill = 0,
        Line,
        Point
    };

    enum class FrontFaceDirection
    {
        CounterClockwise = 0,
        Clockwise
    };

    enum class FillMode
    {
        Wireframe,
        Solid,
        Point
    };

    struct DepthBias
    {
        float depthBias = 0.0f;
        float slopeBias = 0.0f;
        float biasClamp = 0.0f;
    };
    
    class RasterizerState
    {
    public:

        virtual void SetFillMode( FillMode frontFace = FillMode::Solid, FillMode backFace = FillMode::Solid ) = 0;
        virtual void GetFillMode( FillMode& frontFace, FillMode& backFace ) const = 0;

        virtual void SetCullMode( CullFaceMode cullMode = CullFaceMode::Back ) = 0;
        virtual CullFaceMode GetCullMode() const = 0;

        virtual void SetFrontFacing( FrontFaceDirection frontFace = FrontFaceDirection::CounterClockwise ) = 0;
        virtual FrontFaceDirection GetFrontFacing() const = 0;

        virtual void SetDepthBias(const DepthBias& bias = {}) = 0;
        virtual DepthBias GetDepthBias() const = 0;

        virtual void SetDepthClipEnabled( bool depthClipEnabled = true ) = 0;
        virtual bool GetDepthClipEnabled() const = 0;

        virtual void SetScissorEnabled( bool scissorEnable = false ) = 0;
        virtual bool GetScissorEnabled() const = 0;

        virtual void SetScissorRect( const Rect& rect ) = 0;
        virtual void SetScissorRects( const std::vector<Rect>& rects ) = 0;
        virtual const std::vector<Rect>& GetScissorRects() const = 0;

        virtual void SetViewport( const Viewport& viewport ) = 0;
        virtual void SetViewports( const std::vector<Viewport>& viewports ) = 0;
        virtual const std::vector<Viewport>& GetViewports() = 0;
        
        virtual void SetLineWidth(float size) = 0;
        virtual float GetLineWidth() = 0;

        virtual void SetPointSize(float size) = 0;
        virtual float GetPointSize() = 0;

        
        virtual  ~RasterizerState() = default;
    };
    
}
