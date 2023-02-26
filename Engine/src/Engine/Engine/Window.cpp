#include "boidpch.h"
#include "Window.h"

#include "Platform/Windows/WindowsWindow.h"


namespace Polyboid
{
    Unique<Window> Window::Create(const WindowSettings& settings)
    {
        return std::make_unique<WindowsWindow>(settings);
    }
}
