#pragma once
#include "Engine/Engine/PolyboidWindow.h"

namespace Polyboid
{
    class Context
    {
    private:
        GLFWwindow* m_Window = nullptr;

    public:
        Context();
        void MakeCurrent(GLFWwindow* window);
        static std::shared_ptr<Context> MakeContext();
    };

}

