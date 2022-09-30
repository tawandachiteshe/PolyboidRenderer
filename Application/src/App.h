#pragma once
#include <spdlog/details/backtracer.h>

#include "Engine/Core/Application.h"
#include "Engine/Core/Base.h"
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
        Ref<Texture> m_Texture, m_Texture2;
        
        void Update(float deltaTime) override;
    };



}
