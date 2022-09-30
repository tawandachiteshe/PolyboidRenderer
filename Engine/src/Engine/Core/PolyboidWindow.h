#pragma once
#include <memory>
#include <string>

class GLFWwindow;

namespace Polyboid
{

    struct WindowSpecs
    {
        WindowSpecs() = default;
        WindowSpecs(uint32_t Height, uint32_t Width, const std::string& Title): Title(Title), Height(Height),Width(Width){}
        
        std::string Title;
        uint32_t Height, Width;
    };
    
    class PolyboidWindow
    {
    
    private:
        GLFWwindow* m_Window;
       

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

        GLFWwindow* GetNativeWindow() const { return m_Window; }
        
        static std::unique_ptr<PolyboidWindow> MakeWindow(const WindowSpecs& specs);

        bool Run();

        void PollEvents();

        virtual ~PolyboidWindow();
    };

    
}
