#include "boidpch.h"

#include "Input.h"

#include <spdlog/spdlog.h>

#include "Application.h"
#include "GLFW/glfw3.h"


namespace Polyboid
{
    glm::vec2 Input::GetMousePosition()
    {
        double xpos, ypos;
        glfwGetCursorPos(Application::Get()->GetWindow()->GetNativeWindow(), &xpos, &ypos);
        return glm::vec2(xpos, ypos);
    }

    float Input::GetMouseX()
    {
        return GetMousePosition().x;
    }

    float Input::GetMouseY()
    {
        return GetMousePosition().y;
    }

    void Input::SetCursorMode(CursorMode mode)
    {
        glfwSetInputMode(Application::Get()->GetWindow()->GetNativeWindow(), GLFW_CURSOR,  static_cast<int>(mode));
    }

    bool Input::MousePressed(MouseCodes codes)
    {
        const auto window = Application::Get()->GetWindow()->GetNativeWindow();
        const int state = glfwGetMouseButton(window, static_cast<int>(codes));

        return state == static_cast<int>(KeyAction::PRESS);
    }

    bool Input::MouseReleased(MouseCodes codes)
    {
        const auto window = Application::Get()->GetWindow()->GetNativeWindow();
        const int state = glfwGetMouseButton(window, static_cast<int>(codes));

        return state == static_cast<int>(KeyAction::RELEASE);
    }

    bool Input::MouseHeld(MouseCodes codes)
    {
        const auto window = Application::Get()->GetWindow()->GetNativeWindow();
        const int state = glfwGetMouseButton(window, static_cast<int>(codes));

        return state == static_cast<int>(KeyAction::REPEAT);
    }

    bool Input::KeyPressed(KeyCodes codes)
    {
        const auto window = Application::Get()->GetWindow()->GetNativeWindow();
        int state = glfwGetKey(window, static_cast<int>(codes));

        return state == static_cast<int>(KeyAction::PRESS);
    }

    bool Input::KeyReleased(KeyCodes codes)
    {
        const auto window = Application::Get()->GetWindow()->GetNativeWindow();
        const int state = glfwGetKey(window, static_cast<int>(codes));

        return state == static_cast<int>(KeyAction::RELEASE);
    }

    bool Input::KeyHeld(KeyCodes codes)
    {
        const auto window = Application::Get()->GetWindow()->GetNativeWindow();
        const int state = glfwGetKey(window, static_cast<int>(codes));

        return state == static_cast<int>(KeyAction::REPEAT);
    }
}

