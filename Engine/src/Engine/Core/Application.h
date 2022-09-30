#pragma once
#include "PolyboidWindow.h"
#include "Engine/Renderer/Swapchain.h"


int main(int argc, char** argv);
namespace Polyboid
{
    struct ApplicationData
    {
        ApplicationData() = default;
        WindowSpecs windowSpecs;
        ApplicationData(const WindowSpecs& specs): windowSpecs(specs){}
        
    };
    
    class Application
    {
    public:
        Application();
        virtual ~Application();

    protected:
        ApplicationData m_AppData = WindowSpecs(600, 800, "Polyboid");
        
    private:
      
        std::unique_ptr<PolyboidWindow> m_Window;
        std::shared_ptr<Swapchain> m_Swapchain;
        void Run();
        virtual void Update(float deltaTime) = 0;

        double m_DeltaTime = 0.0;
        double m_LastFrame = 0.0;

        
    private:
        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
       
    
    };

    Application* CreateApplication();
}

