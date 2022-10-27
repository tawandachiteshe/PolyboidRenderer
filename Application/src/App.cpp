#include "App.h"

#include <spdlog/spdlog.h>

#include "imgui.h"
#include "Engine/Core/EntryPoint.h"
#include "Engine/Core/ImguiSetup.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/MeshLoader.h"
#include "Engine/Core/ECS/Components.h"
#include "Engine/Core/ECS/GameObject.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Polyboid
{
    App::App()
    {
        m_Camera = std::make_shared<Camera3D>(m_AppData.windowSpecs.Width, m_AppData.windowSpecs.Height);

        m_VA = VertexBufferArray::MakeVertexBufferArray();

    	m_MeshLoader = MeshLoader::LoadFile("Assets/Models/BoxTextured.glb");
        auto indices = m_MeshLoader->GetIndices16()[0].data();
    	verts = m_MeshLoader->GetVertices()[0].data();
        auto vertsSize = m_MeshLoader->GetVertices()[0].size() * sizeof(Vertex);
        auto count = m_MeshLoader->GetIndices16()[0].size();

        m_Count = vertsSize;

        m_Shader = Shader::MakeShader("Assets/Shaders/vert.glsl", "Assets/Shaders/frag.glsl");

        m_IB = IndexBuffer::MakeIndexBuffer(indices, count);
        m_VB = VertexBuffer::MakeVertexBuffer(verts, vertsSize);

        m_VB->DescribeBuffer({
            { BufferComponent::Float3, "aPosition" },
            { BufferComponent::Float3, "aNormal" },
            { BufferComponent::Float2, "aUV"}
            });
        m_VA->AddVertexBuffer(m_VB);
        m_VA->SetIndexBuffer(m_IB);

        m_Texture = Texture::MakeTexture2D("Assets/Textures/checker.jpg");
        m_Texture->Bind();

        m_uniformBuffer = UniformBuffer::MakeUniformBuffer(sizeof(glm::mat4), 2);
        m_framebuffer = Framebuffer::MakeFramebuffer({ m_AppData.windowSpecs.Width, m_AppData.windowSpecs.Height });


        bool demoWin = true;

       
        
    }

    App::~App()
    {
    }

    static double lastTime = 0;
    static int nbFrames = 0;
    static float rotation = 0.f;

    void App::OnKeyEvent(KeyCodes codes, KeyAction action)
    {
        m_Camera->OnKeyEvent(codes, action);
    }

    void App::OnWindowResizeEvent(uint32_t width, uint32_t height)
    {
        Application::OnWindowResizeEvent(width, height);
        Renderer::CreateViewPort({width, height});
        m_Camera->OnWindowResize(width, height);
    }

    void App::OnMouseScrollEvent(double offset)
    {
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



        ImGui::Begin("Window is lit");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::End();

        m_framebuffer->Bind();
        //Clear first
        Renderer::Clear();

      
        Renderer2D::BeginDraw(m_Camera);
        Renderer2D::DrawQuad({ 0.0f , 0.0f, 0.0f }, { .23, 0.23, 0.98, 1.0f });
        Renderer2D::DebugWindow();
        Renderer2D::EndDraw();

        m_framebuffer->UnBind();

        Renderer::Clear();

        //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        ImGui::Begin("GameWindow");
        {
            // Using a Child allow to fill all the space of the window.
            // It also alows customization
            ImGui::BeginChild("GameRender");
            // Get the size of the child (i.e. the whole draw size of the windows).
            ImVec2 wsize = ImGui::GetWindowSize();
            // Because I use the texture from OpenGL, I need to invert the V from the UV.
            ImGui::Image((ImTextureID)m_framebuffer->GetColorAttachment0TextureID(), wsize, ImVec2(0, 1), ImVec2(1, 0));
            ImGui::EndChild();
        }
        ImGui::End();
        
       
  
        Renderer2D::BeginDraw(m_Camera);
        Renderer2D::DrawQuad({ 0.0f , 0.0f, 0.0f });
        Renderer2D::DebugWindow();
        Renderer2D::EndDraw();


        glm::mat4 translate = glm::translate(glm::mat4(1.0f), { 0.0f, 2.0f, 0.0f });

        Renderer::Submit(m_VA, m_Shader, translate);
        Renderer::BeginDraw(m_Camera);
        Renderer::DrawIndexed();
        Renderer::EndDraw();

    }


    Application* CreateApplication()
    {
        return new App();
    }
}
