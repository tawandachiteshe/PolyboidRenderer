#include "boidpch.h"

#include "Context.h"

#include <memory>
#include <spdlog/spdlog.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"


namespace Polyboid
{
    Context::Context()
    {
    }

    void Context::MakeCurrent(GLFWwindow* window)
    {
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
        return std::make_shared<Context>();
    }
}
