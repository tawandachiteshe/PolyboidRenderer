#pragma once
#include "Engine/Renderer/RasterizerState.h"

namespace Polyboid
{
    class GLRasterizerState : public RasterizerState
    {
    private:
        DepthBias m_DepthBias = {};
        FillMode m_FrontFaceFillMode = FillMode::Solid;
        FillMode m_BackFaceFillMode = FillMode::Solid;
        CullFaceMode m_CullMode = CullFaceMode::Back;

        FrontFaceDirection m_FrontFace = FrontFaceDirection::CounterClockwise;

        bool m_DepthClipEnabled = true;
        bool m_ScissorEnabled = false;

        bool m_MultisampleEnabled = false;
        bool m_AntialiasedLineEnabled = false;
        
        std::vector<Viewport> m_Viewports;
        std::vector<Rect> m_ScissorRects;

        //Only Works with opengl
        float m_LineWidth = 2.0f;
        float m_PointSize = 1.0f;

        bool m_StateDirty = true;
        bool m_ScissorRectsDirty = true;
        bool m_ViewportsDirty = true;
        
        
    public:
        GLRasterizerState();
        
        void SetFillMode(FillMode frontFace, FillMode backFace) override;
        void GetFillMode(FillMode& frontFace, FillMode& backFace) const override;

        void SetCullMode(CullFaceMode cullMode) override;
        CullFaceMode GetCullMode() const override;

        void SetFrontFacing(FrontFaceDirection frontFace) override;
        FrontFaceDirection GetFrontFacing() const override;

        void SetDepthBias(const DepthBias& bias) override;
        DepthBias GetDepthBias() const override;

        void SetDepthClipEnabled(bool depthClipEnabled) override;
        bool GetDepthClipEnabled() const override;

        void SetScissorEnabled(bool scissorEnable) override;
        bool GetScissorEnabled() const override;

        void SetScissorRect(const Rect& rect) override;
        void SetScissorRects(const std::vector<Rect>& rects) override;

        const std::vector<Rect>& GetScissorRects() const override;
        void SetViewport(const Viewport& viewport) override;

        void SetViewports(const std::vector<Viewport>& viewports) override;
        const std::vector<Viewport>& GetViewports() override;

        void SetLineWidth(float size) override;
        float GetLineWidth() override;

    	void SetPointSize(float size) override;
        float GetPointSize() override;

        virtual void Bind();
        
        ~GLRasterizerState() override = default;
    };

}

