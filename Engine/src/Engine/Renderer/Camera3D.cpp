#include "boidpch.h"
#include "Camera3D.h"


#include <spdlog/spdlog.h>

#include "Engine/Engine/Input.h"

#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Polyboid
{
    Camera3D::Camera3D(uint32_t width, uint32_t height) : m_Width(width), m_Height(height)
    {
  
        auto cameraDirection = glm::normalize(m_Position - m_Target);
        auto up = glm::vec3(0.0f, 1.0f, 0.0f);
        auto cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        auto cameraUp = glm::cross(cameraDirection, cameraRight);
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.f, 0.f, 3.f),
            glm::vec3(0.0f, 0.f, 0.f), {0.f, 1.f, 0.f});

        ReCalculateViewMat();
    }

    void Camera3D::SetZoomAmount(float dir)
    {

        m_Fov -= dir * 2.f;

        if (m_Fov < 1.0f)
        {
            m_Fov = 1.0f;
        }

        if (m_Fov > 45.0f)
        {
            m_Fov = 45.0f;
        }
        
        ReCalculateViewMat();
    }

    void Camera3D::ReCalculateViewMat()
    {
 
        m_Aspect = static_cast<float>(m_Width) / static_cast<float>(m_Height);
        auto center = m_Position + m_Front;

        //idk why i named it this....
        m_View = glm::perspective(glm::radians(m_Fov), m_Aspect, m_Near, m_Far) * glm::lookAt(
            m_Position, center, m_CameraUp);

        m_Projection = m_View;
    }

    void Camera3D::Update(float deltaTime)
    {
        if (Input::KeyPressed(KeyCodes::W))
        {
            m_Position += m_Speed * m_Front * deltaTime;
        }

        if (Input::KeyPressed(KeyCodes::S))
        {
            m_Position -= m_Speed * m_Front * deltaTime;
        }

        if (Input::KeyPressed(KeyCodes::Q))
        {
            m_Position -= glm::vec3(0, 1, 0) * m_Speed * deltaTime;
        }

        if (Input::KeyPressed(KeyCodes::E))
        {
            m_Position += glm::vec3(0, 1, 0) * m_Speed * deltaTime;
        }

        if (Input::KeyPressed(KeyCodes::A))
        {
            m_Position -= glm::normalize(glm::cross(m_Front, m_CameraUp)) * m_Speed * deltaTime;
        }

        if (Input::KeyPressed(KeyCodes::D))
        {
            m_Position += glm::normalize(glm::cross(m_Front, m_CameraUp)) * m_Speed * deltaTime;
        }

        ReCalculateViewMat();
    }

    
    void Camera3D::OnMouseMove()
    {

        auto mousePosX = Input::GetMouseX();
        auto mousePosY = Input::GetMouseY();

        if (m_FirstClick)
        {
            m_LastMousePosX = mousePosX;
            m_LastMousePosY = mousePosY;
            m_FirstClick = false;
        }
      
        auto xoffset = mousePosX - m_LastMousePosX;
        auto yoffset = m_LastMousePosY - mousePosY;
        
        m_LastMousePosX = mousePosX;
        m_LastMousePosY = mousePosY;
        
        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (m_Pitch > 89.0f)
        {
            m_Pitch = 89.0f;
        }

        if (m_Pitch < -89.0f)
        {
            m_Pitch = -89.0f;
        }

        glm::vec3 direction = glm::vec3(0.f);

        direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        direction.y = sin(glm::radians(m_Pitch));
        direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

        m_Front = glm::normalize(direction);

        ReCalculateViewMat();

        spdlog::info("{0}, {1}, {2}", m_Position.x, m_Position.y, m_Position.z);

    }

    void Camera3D::OnKeyEvent(KeyCodes codes, KeyAction action)
    {

        if (action == KeyAction::PRESS)
        {
            if (codes == KeyCodes::LEFT_ALT)
            {
                Input::SetCursorMode(CursorMode::DISABLED);
                m_FirstClick = true;
            }
        }
        else if (action == KeyAction::RELEASE)
        {
            if (codes == KeyCodes::LEFT_ALT)
            {
                Input::SetCursorMode(CursorMode::NORMAL);
                m_FirstClick = false;
            }
        }
    }

    void Camera3D::OnWindowResize(uint32_t width, uint32_t height)
    {
        m_Aspect = static_cast<float>(width) / static_cast<float>(height);
    }
}
