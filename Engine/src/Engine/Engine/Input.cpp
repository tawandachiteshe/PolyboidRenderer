#include "boidpch.h"

#include "Input.h"

#include <spdlog/spdlog.h>

#include "Application.h"
#include "Debug/Profiler.h"
#include "GLFW/glfw3.h"


namespace Polyboid
{
    glm::vec2 Input::GetMousePosition()
    {
        POLYBOID_PROFILE_FUNCTION();
        double xpos, ypos;
        glfwGetCursorPos(Application::Get()->GetWindow()->GetNativeWindow(), &xpos, &ypos);
        return glm::vec2(xpos, ypos);
    }

    float Input::GetMouseX()
    {
        POLYBOID_PROFILE_FUNCTION();
        return GetMousePosition().x;
    }

    float Input::GetMouseY()
    {
        POLYBOID_PROFILE_FUNCTION();
        return GetMousePosition().y;
    }

    void Input::SetCursorMode(CursorMode mode)
    {
        POLYBOID_PROFILE_FUNCTION();
        glfwSetInputMode(Application::Get()->GetWindow()->GetNativeWindow(), GLFW_CURSOR,  static_cast<int>(mode));
    }

    bool Input::MousePressed(MouseCodes codes)
    {
        POLYBOID_PROFILE_FUNCTION();
        const auto window = Application::Get()->GetWindow()->GetNativeWindow();
        const int state = glfwGetMouseButton(window, static_cast<int>(codes));

        return state == static_cast<int>(KeyAction::PRESS);
    }

    bool Input::MouseReleased(MouseCodes codes)
    {
        POLYBOID_PROFILE_FUNCTION();
        const auto window = Application::Get()->GetWindow()->GetNativeWindow();
        const int state = glfwGetMouseButton(window, static_cast<int>(codes));

        return state == static_cast<int>(KeyAction::RELEASE);
    }

    bool Input::MouseHeld(MouseCodes codes)
    {
        POLYBOID_PROFILE_FUNCTION();
        const auto window = Application::Get()->GetWindow()->GetNativeWindow();
        const int state = glfwGetMouseButton(window, static_cast<int>(codes));

        return state == static_cast<int>(KeyAction::REPEAT);
    }

    bool Input::KeyPressed(KeyCodes codes)
    {
        POLYBOID_PROFILE_FUNCTION();
        const auto window = Application::Get()->GetWindow()->GetNativeWindow();
        int state = glfwGetKey(window, static_cast<int>(codes));

        return state == static_cast<int>(KeyAction::PRESS);
    }

    bool Input::KeyReleased(KeyCodes codes)
    {
        POLYBOID_PROFILE_FUNCTION();
        const auto window = Application::Get()->GetWindow()->GetNativeWindow();
        const int state = glfwGetKey(window, static_cast<int>(codes));

        return state == static_cast<int>(KeyAction::RELEASE);
    }

    bool Input::KeyHeld(KeyCodes codes)
    {
        POLYBOID_PROFILE_FUNCTION();
        const auto window = Application::Get()->GetWindow()->GetNativeWindow();
        const int state = glfwGetKey(window, static_cast<int>(codes));

        return state == static_cast<int>(KeyAction::REPEAT);
    }
}

