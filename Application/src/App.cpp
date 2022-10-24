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

namespace Polyboid
{
    App::App()
    {
        m_Camera = std::make_shared<Camera3D>(m_AppData.windowSpecs.Width, m_AppData.windowSpecs.Height);

        m_VA = VertexBufferArray::MakeVertexBufferArray();

        m_MeshLoader = MeshLoader::LoadFile("Assets/Models/moreverts.glb");
        auto indices = m_MeshLoader->GetIndices32()[0].data();
    	verts = m_MeshLoader->GetVertices()[0].data();
        auto vertsSize = m_MeshLoader->GetVertices()[0].size() * sizeof(Vertex);
        auto count = m_MeshLoader->GetIndices32()[0].size();

        m_Count = vertsSize;

        m_Shader = Shader::MakeShader("Assets/Shaders/vert.glsl", "Assets/Shaders/frag.glsl");

        m_IB = IndexBuffer::MakeIndexBuffer(indices, count);
        m_VB = VertexBuffer::MakeVertexBuffer(vertsSize);

        m_VB->DescribeBuffer({
            { BufferComponent::Float3, "aPosition" },
            { BufferComponent::Float3, "aNormal" },
            { BufferComponent::Float2, "aUV"}
            });
        m_VA->AddVertexBuffer(m_VB);
        m_VA->SetIndexBuffer(m_IB);

        m_Texture = Texture::MakeTexture2D("Assets/Textures/checker.jpg");
        m_Texture->Bind();
        
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



        ImGui::Begin("Window is lit");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::End();

        //Clear first
        Renderer::Clear();

        /*
       glm::mat4 translate = glm::translate(glm::mat4(1.0f), { 0.0f, 2.0f, 0.0f });

        Renderer::Submit(m_VA, m_Shader, translate);
     

        Renderer::BeginDraw(m_Camera);
        Renderer::DrawIndexed();
        m_VB->SetData(m_MeshLoader->GetVertices()[0].data());
        Renderer::EndDraw();
        */

        
        Renderer2D::BeginDraw(m_Camera);
        for (int x = 0; x < 200; ++x)
        {
	        for (int y = 0; y < 200; ++y)
	        {
               Renderer2D::DrawQuad({ x , y, 0.0f });
	        }

        }
        Renderer2D::DebugWindow();
        Renderer2D::EndDraw();
        
  

    }


    Application* CreateApplication()
    {
        return new App();
    }
}
