#pragma once
#include <cstdint>

#include "glm/matrix.hpp"

namespace Polyboid
{
    class Camera3D
    {
    private:
        uint32_t m_Width, m_Height;
        float m_Fov = 45.f;
        float m_Speed = 5.f;
        float m_Near = 0.01f;
        float m_Far = 1000.f;
        float m_Aspect = 0.f;

        float m_LastMousePosX =  0.f;
        float m_LastMousePosY = 0.f;
        float m_Yaw = -90.f;
        float m_Pitch = 0.f;
        double m_Zoom = 2.f;

        bool m_FirstClick = true;
     
        
      
        glm::vec3 m_Position = glm::vec3(0, 0, 10);
        glm::mat4 m_Transform = glm::mat4(1.f);
        glm::mat4 m_View = glm::mat4(1.f);
        glm::vec3 m_Target = glm::vec3(0);
        glm::vec3 m_Front = { 0.f, 0.f, -1.f };
        glm::vec3 m_CameraUp = { .0f, 1.0f, .0f };

        void ReCalculateViewMat();

        
    
    
    public:
        Camera3D(uint32_t width, uint32_t height);

        void SetFirstClick(bool firstClick) { m_FirstClick = firstClick; }
        void SetZoomAmount(float dir);
        const glm::mat4& GetViewProjectionMatrix() const { return m_View; }
        void SetTarget(const glm::vec3& position) { m_Target = position; }
        void Update(float deltaTime);
        void OnWindowResize(uint32_t width, uint32_t height);
        void OnMouseMove();
    };

}

