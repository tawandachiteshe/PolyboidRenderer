#pragma once
#include "Engine/Engine/Window.h"


struct GLFWwindow;

namespace Polyboid
{

    class WindowsWindow : public Window
    {

    private:
        WindowSettings m_Settings;
        WindowData m_Data;
        GLFWwindow* m_Handle = nullptr;
        static WindowData& GetWindowData(GLFWwindow* window);
        
    public:
        WindowsWindow(const WindowSettings& settings);
        uint32_t GetWidth() override;
        uint32_t GetHeight() override;
        void PollEvents() override;

        void SetEventCallback(const EventFn& fn) override;
        std::any GetNativeWindow() override;
        ~WindowsWindow() override;
    };
}


