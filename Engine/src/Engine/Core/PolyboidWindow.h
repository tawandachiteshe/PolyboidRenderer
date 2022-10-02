#pragma once
#include <functional>
#include <memory>
#include <string>

#include "KeyCodes.h"

extern "C" {
    typedef struct GLFWwindow GLFWwindow;
}


namespace Polyboid
{

    struct WindowSpecs
    {
        WindowSpecs() = default;
        WindowSpecs(uint32_t Width, uint32_t Height, const std::string& Title): Title(Title), Height(Height),Width(Width){}
        
        std::string Title = "";
        uint32_t Height = 0, Width = 0;
    };

    struct WindowData
    {
        std::function<void(KeyCodes, KeyAction)> OnKeyEvent;
        std::function<void(MouseCodes, KeyAction)> OnMouseEvent;
        std::function<void(uint32_t, uint32_t)> OnFrameBufferResizeEvent;
        std::function<void(uint32_t, uint32_t)> OnWindowResizeEvent;
        std::function<void()> OnWindowCloseEvent;
        std::function<void(double)> OnMouseScrollEvent;
    };
    
    class PolyboidWindow
    {
    
    private:
        GLFWwindow* m_Window;
        static  std::unique_ptr<WindowData> s_WindowData;

    public:

        PolyboidWindow(const WindowSpecs& specs);
        
        bool m_IsRunning = false;

        void Close() { m_IsRunning = false; }

        bool IsRunning() const {
            if (!m_IsRunning)
            {
                
            }

            return m_IsRunning;

        }

        [[nodiscard]] GLFWwindow* GetNativeWindow() const { return m_Window; }
        
        static std::unique_ptr<PolyboidWindow> MakeWindow(const WindowSpecs& specs);

        static std::unique_ptr<WindowData>& GetWindowData() { return s_WindowData; }

        static void PollEvents();

        virtual ~PolyboidWindow();
    };

    
}
