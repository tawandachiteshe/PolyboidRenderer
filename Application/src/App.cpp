#include "App.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include "Engine/Core/EntryPoint.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Renderer/VertexBufferArray.h"


namespace Polyboid
{
    App::App()
    {
        m_VA = VertexBufferArray::MakeVertexBufferArray();

        float verts[4 * 9] = {
            -.5f, -.5f, 0.0f, .23f, .45f, .98f, 1.0f, 0.0f, 0.0f,
            -.5f, .5f, 0.0f, .93f, .45f, .98f, 1.0f, 0.0f, 1.0f,
            .5f, -.5f, 0.0f, .43f, .75f, .98f, 1.0f, 1.0f, 0.0f,
            .5f,  .5f, 0.0f, .43f, .75f, .98f, 1.0f, 1.0f, 1.0f,
        };

        uint32_t indices[6] = {
            0 , 1, 2, 3, 1, 2
        };

        m_IB = IndexBuffer::MakeIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t));
        m_VB = VertexBuffer::MakeVertexBuffer(verts, sizeof(verts));
        m_VB->DescribeBuffer(
            {
                {BufferComponent::Float3, "aPosition"},
                {BufferComponent::Float4, "aColor"},
                {BufferComponent::Float2, "aUV"}
            }
        );
        m_VA->AddVertexBuffer(m_VB);
        m_VA->SetIndexBuffer(m_IB);
        //
        m_Shader = Shader::MakeShader("Assets/Shaders/vert.glsl", "Assets/Shaders/frag.glsl");
        m_Texture = Texture::MakeTexture2D("Assets/Textures/checker.jpg");
        m_Texture2 = Texture::MakeTexture2D(1, 1);
        uint32_t color = 0xFF0000FF;
        m_Texture2->SetData(&color, sizeof(color));
        std::array<int, 32> u_Textures = {};
        u_Textures.fill(0);

        m_Shader->Bind();
        m_Texture->Bind(0);
        m_Texture2->Bind(1);
        
        m_Shader->SetIntArray("u_Textures", u_Textures.data(), 32);
        m_Shader->SetInt("uTexture", 0);
        m_Shader->SetInt("uTexture2", 1);
    }

    App::~App()
    {
    }

    static double lastTime = 0;
    static int nbFrames = 0;

    void App::Update(float deltaTime)
    {
        lastTime += deltaTime;

        if (lastTime >= 1.0)
        {
            nbFrames++;

            lastTime = 0;
        }
        //Clear first
        Renderer::Clear();
        Renderer::CreateViewPort({800, 600});
        Renderer::Submit(m_VA, m_Shader);
        Renderer::Draw();

        // spdlog::info("FPS {0}", 1 / deltaTime );
    }


    Application* CreateApplication()
    {
        return new App();
    }
}
