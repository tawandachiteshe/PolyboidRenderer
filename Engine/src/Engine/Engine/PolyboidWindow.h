#pragma once
#include <functional>
#include <memory>
#include <string>

#include "KeyCodes.h"
#include "Events/Event.h"
//#include "Events/Event.h"

extern "C" {
    typedef struct GLFWwindow GLFWwindow;
}


namespace Polyboid
{

    struct WindowSpecs
    {
        WindowSpecs() = default;
        WindowSpecs(uint32_t Width, uint32_t Height, const std::string& Title): Title(Title), Height(Height),Width(Width){}
        
        std::string Title;
        uint32_t Height = 0, Width = 0;

        float GetAspectRatio() { return (float)Height / Width; }

    };

    struct WindowData
    {
        EventFn eventfn;
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
