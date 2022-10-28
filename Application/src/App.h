#pragma once
#include <spdlog/details/backtracer.h>

#include "Engine/Engine/Application.h"
#include "Engine/Engine/Base.h"
#include "Engine/Engine/MeshLoader.h"
#include "Engine/Renderer/Camera3D.h"
#include "Engine/Renderer/IndexBuffer.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture2D.h"
#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include <Engine/Renderer/UniformBuffer.h>
#include <Engine/Renderer/Framebuffer.h>

namespace Polyboid
{

    class App : public Application
    {
    public:
        App();
        ~App() override;
    private:

        Ref<UniformBuffer> m_uniformBuffer;
        Ref<Shader> m_Shader;
        Ref<VertexBufferArray> m_VA;
        Ref<VertexBuffer> m_VB;
        Ref<IndexBuffer> m_IB;
        std::vector<Ref<VertexBuffer>> m_VBs;
        std::vector<Ref<IndexBuffer>> m_IBs;
        Ref<Texture> m_Texture, m_Texture2, m_Texture3;
        Ref<Camera3D> m_Camera = nullptr;
        bool m_CanUseMouse = false;
        void OnKeyEvent(KeyCodes codes, KeyAction action);
        void OnWindowResizeEvent(uint32_t width, uint32_t height);
        void OnMouseScrollEvent(double offset);
        Vertex* verts = nullptr;
        uint32_t m_Count = 0;
        Ref<MeshLoader> m_MeshLoader;
        Ref<Framebuffer> m_framebuffer;
    };



}
