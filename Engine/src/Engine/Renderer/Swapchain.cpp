#include "boidpch.h"

#include "Swapchain.h"

#include "Platform/Opengl/GLSwapchain.h"


namespace Polyboid
{
    //Quick impl to test things out
    Ref<Swapchain> Swapchain::Create(const std::any& window)
    {
        return std::make_shared<GLSwapchain>(window);
    }
}

