#include "boidpch.h"

#include "Context.h"

#include <memory>
#include <spdlog/spdlog.h>

#include "Engine/Engine/Debug/Profiler.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


namespace Polyboid
{
    Context::Context()
    {
        POLYBOID_PROFILE_FUNCTION();
    }

    void Context::MakeCurrent(GLFWwindow* window)
    {
        POLYBOID_PROFILE_FUNCTION();
        glfwMakeContextCurrent(window);

        int isInit = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (!isInit)
        {
            __debugbreak();
        }

        const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
        const GLubyte* version = glGetString(GL_VERSION); // version as a string
        spdlog::info("Renderer: {0}", renderer);
        spdlog::info("OpenGL version supported: {0}", version);
        
    }

    std::shared_ptr<Context> Context::MakeContext()
    {
        POLYBOID_PROFILE_FUNCTION();
        return std::make_shared<Context>();
    }
}
