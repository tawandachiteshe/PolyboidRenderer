#include "boidpch.h"

#include "Swapchain.h"

#include "RenderAPI.h"
#include "Engine/Engine/Application.h"


namespace Polyboid
{
    //Quick impl to test things out
    Ref<Swapchain> Swapchain::Create(const SwapchainSettings& settings)
    {
        return RenderAPI::Get()->CreateSwapChain(settings);
    }
}

