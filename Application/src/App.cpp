#include "App.h"

#include <spdlog/spdlog.h>

#include "imgui.h"
#include "Engine/Core/EntryPoint.h"
#include "Engine/Core/ImguiSetup.h"
#include "Engine/Core/Input.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Polyboid
{
    App::App()
    {
        m_VA = VertexBufferArray::MakeVertexBufferArray();

        float verts[4 * 9] = {
            -.5f, -.5f, 0.0f, .23f, .45f, .98f, 1.0f, 0.0f, 0.0f,
            .5f, -.5f, 0.0f, .93f, .45f, .98f, 1.0f, 1.0f, 0.0f,
            -.5f, .5f, 0.0f, .43f, .75f, .98f, 1.0f, 0.0f, 1.0f,
            .5f, .5f, 0.0f, .43f, .75f, .98f, 1.0f, 1.0f, 1.0f,
        };

        uint32_t indices[6] = {
            0, 2, 3, 0, 3, 1,
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
        uint32_t color[4] = {0xFF0000FF, 0xFF00FFFF, 0xFF0000FF, 0xFF00FFFF};
        m_Texture2->SetData(&color, sizeof(color));
        std::array<int, 32> u_Textures = {};
        u_Textures.fill(0);

        m_Shader->Bind();
        m_Texture->Bind(0);
        m_Texture2->Bind(1);

        m_Shader->SetIntArray("u_Textures", u_Textures.data(), 32);
        m_Shader->SetInt("uTexture", 0);
        m_Shader->SetInt("uTexture2", 1);

        m_Camera = std::make_shared<Camera3D>(m_AppData.windowSpecs.Width, m_AppData.windowSpecs.Height);
    }

    App::~App()
    {
    }

    static double lastTime = 0;
    static int nbFrames = 0;
    static float rotation = 0.f;

    void App::OnKeyEvent(KeyCodes codes, KeyAction action)
    {
        Application::OnKeyEvent(codes, action);

        if (action == KeyAction::PRESS)
        {
            if (codes == KeyCodes::LEFT_ALT)
            {
                Input::SetCursorMode(CursorMode::DISABLED);
                m_CanUseMouse = true;
            }
        }
        else if (action == KeyAction::RELEASE)
        {
            if (codes == KeyCodes::LEFT_ALT)
            {
                Input::SetCursorMode(CursorMode::NORMAL);
                m_CanUseMouse = false;
                m_Camera->SetFirstClick(true);
            }
        }
        
        
    }

    void App::OnWindowResizeEvent(uint32_t width, uint32_t height)
    {
        Application::OnWindowResizeEvent(width, height);
        Renderer::CreateViewPort({ width, height });
        m_Camera->OnWindowResize(width, height);
    }

    void App::OnMouseScrollEvent(double offset)
    {
        Application::OnMouseScrollEvent(offset);
        m_Camera->SetZoomAmount(static_cast<float>(offset));
    }


    void App::Update(float deltaTime)
    {
        lastTime += deltaTime;

        if (lastTime >= 1.0)
        {
            nbFrames++;

            lastTime = 0;
        }

        rotation += deltaTime * 5.f;

        m_Camera->Update(deltaTime);

        if (m_CanUseMouse)
        {
            m_Camera->OnMouseMove();
        }
        
        
        glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(.5f, .5f, 1.f));
        glm::mat4 rotationMat = glm::rotate(glm::mat4(1), rotation, glm::vec3(0.f, 0.f, 1.f));
        glm::mat4 translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(.5, 1, 2.5f));
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-.5, 0, 0.f));

        ImGui::Begin("Window is lit");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        //Clear first
        Renderer::Clear();
        Renderer::Submit(m_VA, m_Shader);
        Renderer::BeginDraw(m_Camera);
        
        Renderer::Draw(translate * scale * rotationMat);
        Renderer::Draw(translate2 * scale * rotationMat);
    }


    Application* CreateApplication()
    {
        return new App();
    }
}
