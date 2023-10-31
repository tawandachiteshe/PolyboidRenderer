#pragma once
#include <any>

#include "Base.h"
#include "Events/Event.h"


namespace Polyboid
{
    struct WindowSettings
    {
        bool IsMainWindow = true;
        uint32_t Width = 0;
        uint32_t Height = 0;
        bool IsVisible = true;
        bool NoApi = false;
        std::any WindowShareHandle;
        std::string WindowTitle;

        WindowSettings(bool is_main_window, uint32_t width, uint32_t height, const std::string& window_title)
            : IsMainWindow(is_main_window),
              Width(width),
              Height(height),
              WindowTitle(window_title)
        {
        }

        WindowSettings(const WindowSettings& other)
            : IsMainWindow(other.IsMainWindow),
              Width(other.Width),
              Height(other.Height),
              IsVisible(other.IsVisible),
              WindowShareHandle(other.WindowShareHandle),
              WindowTitle(other.WindowTitle)
        {
        }

        WindowSettings(WindowSettings&& other) noexcept
            : IsMainWindow(other.IsMainWindow),
              Width(other.Width),
              Height(other.Height),
              IsVisible(other.IsVisible),
              WindowShareHandle(std::move(other.WindowShareHandle)),
              WindowTitle(std::move(other.WindowTitle))
        {
        }

        WindowSettings& operator=(const WindowSettings& other)
        {
            if (this == &other)
                return *this;
            IsMainWindow = other.IsMainWindow;
            Width = other.Width;
            Height = other.Height;
            IsVisible = other.IsVisible;
            WindowShareHandle = other.WindowShareHandle;
            WindowTitle = other.WindowTitle;
            return *this;
        }

        WindowSettings& operator=(WindowSettings&& other) noexcept
        {
            if (this == &other)
                return *this;
            IsMainWindow = other.IsMainWindow;
            Width = other.Width;
            Height = other.Height;
            IsVisible = other.IsVisible;
            WindowShareHandle = std::move(other.WindowShareHandle);
            WindowTitle = std::move(other.WindowTitle);
            return *this;
        }
        
        WindowSettings() = default;
    };

    //glfw system
    struct WindowData
    {
        EventFn EventCallback;
    };
    
    class Window
    {
    public:
        virtual void SetEventCallback(const EventFn& fn) = 0;
        virtual std::any GetNativeWindow() = 0;
        virtual uint32_t GetWidth() = 0;
        virtual uint32_t GetHeight() = 0;
        virtual void PollEvents() = 0;
        
        static std::unique_ptr<Window> Create(const WindowSettings& settings);
        virtual ~Window() = default;
    };

}

