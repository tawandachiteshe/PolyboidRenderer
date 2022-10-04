#include "EventSystem.h"


namespace Polyboid
{
    void EventSystem::Init()
    {
        PolyboidWindow::GetWindowData()->OnKeyEvent = [&] (KeyCodes key_codes, KeyAction action)
        {
            
        };

        PolyboidWindow::GetWindowData()->OnMouseEvent = [&](MouseCodes codes, KeyAction action)
        {
            
        };

        PolyboidWindow::GetWindowData()->OnWindowCloseEvent = [&]()
        {
            
        };

        PolyboidWindow::GetWindowData()->OnFrameBufferResizeEvent = [&](uint32_t width, uint32_t height)
        {
           
        };

        PolyboidWindow::GetWindowData()->OnWindowResizeEvent = [&](uint32_t width, uint32_t height)
        {
           
        };

        PolyboidWindow::GetWindowData()->OnMouseScrollEvent = [&](double offset)
        {
           
        };

    }
}
