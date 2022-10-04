#pragma once
#include <spdlog/details/backtracer.h>

#include "Engine/Core/Application.h"
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Camera3D.h"
#include "Engine/Renderer/IndexBuffer.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture2D.h"
#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Renderer/VertexBufferArray.h"

namespace Polyboid
{

    class App : public Application
    {
    public:
        App();
        ~App() override;
    private:

        Ref<Shader> m_Shader;
        Ref<VertexBufferArray> m_VA;
        Ref<VertexBuffer> m_VB;
        Ref<IndexBuffer> m_IB;
        Ref<Texture> m_Texture, m_Texture2, m_Texture3;
        Ref<Camera3D> m_Camera = nullptr;
        bool m_CanUseMouse = false;
        void OnKeyEvent(KeyCodes codes, KeyAction action) override;
        void OnWindowResizeEvent(uint32_t width, uint32_t height) override;
        void OnMouseScrollEvent(double offset) override;
        
        void Update(float deltaTime) override;
    };



}
