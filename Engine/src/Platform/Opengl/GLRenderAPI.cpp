#include "boidpch.h"
#include "GLRenderAPI.h"
#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "GLTexture2D.h"
#include "GLTexture3D.h"
#include "GLUniformBuffer.h"
#include "GLFramebuffer.h"
#include "GLIndexBuffer.h"
#include "GLPipelineState.h"
#include "GLRenderbuffer.h"
#include "GLRenderPass.h"
#include "GLSamplerState.h"
#include "GLSwapchain.h"
#include "GLVertexBuffer.h"
#include "GLVertexBufferArray.h"
#include "GLFW/glfw3.h"

namespace Polyboid
{

#define   API_ALLOC(Klass, ...) std::make_shared<Klass>(__VA_ARGS__)

    GLenum ConvertToOpenGL(PrimitiveType primitiveType)
    {
        switch (primitiveType)
        {
        case PrimitiveType::Points:
            return GL_POINTS;
        case PrimitiveType::LineStrip:
            return GL_LINE_STRIP;
        case PrimitiveType::LineLoop:
            return GL_LINE_LOOP;
        case PrimitiveType::Lines:
            return GL_LINES;
        case PrimitiveType::TriangleStrip:
            return GL_TRIANGLE_STRIP;
        case PrimitiveType::TriangleFan:
            return GL_TRIANGLE_FAN;
        case PrimitiveType::Triangles:
            return GL_TRIANGLES;
        case PrimitiveType::Patches:
            return GL_PATCHES;
        }

        return 0;
    }

    GLenum ConvertToOpenGL(IndexDataType indexDataType)
    {
        switch (indexDataType)
        {
        case IndexDataType::UnsignedShort:
            return GL_UNSIGNED_SHORT;
        case IndexDataType::UnsignedInt:
            return GL_UNSIGNED_INT;
        }

        return 0; 
    }

    void OpenGLMessageCallback(
        unsigned source,
        unsigned type,
        unsigned id,
        unsigned severity,
        int length,
        const char* message,
        const void* userParam)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:         spdlog::critical(message); return;
        case GL_DEBUG_SEVERITY_MEDIUM:       spdlog::error(message); return;
        case GL_DEBUG_SEVERITY_LOW:          spdlog::warn(message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: spdlog::trace(message); return;
        }

       // errrotr code her
    }


    void GLRenderAPI::BeginFrame()
    {
    }

    void GLRenderAPI::EndFrame()
    {
    }

    GLRenderAPI::GLRenderAPI(const std::any& windowHandle): m_Window(windowHandle)
    {
        glfwMakeContextCurrent(std::any_cast<GLFWwindow*>(windowHandle));
        int isInit = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (!isInit)
        {
            __debugbreak();
        }

        const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
        const GLubyte* version = glGetString(GL_VERSION); // version as a string
        spdlog::info("Renderer: {0}", renderer);
        spdlog::info("OpenGL version supported: {0}", version);


        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        // glEnable(GL_COLOR_LOGIC_OP);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        // glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        glEnable(GL_MULTISAMPLE);

        // During init, enable debug output
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(OpenGLMessageCallback, 0);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
        
    }

    Ref<CommandList> GLRenderAPI::CreateCommandList()
    {
        return nullptr;
    }

    // void GLRenderAPI::DrawIndexed(const PrimitiveType& primitiveType, const IndexDataType& indexDataType,
    //                               uint32_t count)
    // {
    //     glDrawElements(ConvertToOpenGL(primitiveType), count, ConvertToOpenGL(indexDataType), nullptr);
    // }
    //
    // void GLRenderAPI::DrawArrays(const PrimitiveType& primitiveType, uint32_t vertexCount)
    // {
    //     glDrawArrays(ConvertToOpenGL(primitiveType), 0, vertexCount);
    // }
    //
    // void GLRenderAPI::Dispatch(const glm::uvec3& groups)
    // {
    //     glDispatchCompute(groups.x, groups.y, groups.z);
    // }

    Ref<Texture> GLRenderAPI::CreateTexture2D(const TextureSettings& settings)
    {
        return std::make_shared<GLTexture2D>(settings);
    }

    Ref<Texture3D> GLRenderAPI::CreateTexture3D(const void** data, const TextureSettings& settings)
    {
        return std::make_shared<GLTexture3D>(data, settings);
    }

    Ref<UniformBuffer> GLRenderAPI::CreateUniformBuffer(uint32_t size, uint32_t binding)
    {
        return std::make_shared<GLUniformBuffer>(size, binding);
    }

    Ref<Framebuffer> GLRenderAPI::CreateFrameBuffer(const FramebufferSettings& settings)
    {
        return std::make_shared<GLFramebuffer>(settings);
    }

    Ref<IndexBuffer> GLRenderAPI::CreateIndexBuffer(const IndexDataType& type, uint32_t count,
	    const std::variant<uint32_t*, uint16_t*>& data)
    {
        return std::make_shared<GLIndexBuffer>(type, count, data);
    }

    Ref<Texture> GLRenderAPI::CreateTexture2D(const std::any& handle)
    {
        return nullptr;
    }

    Ref<Framebuffer> GLRenderAPI::CreateFrameBuffer(const FramebufferSettings& settings,
	    const Ref<RenderPass>& renderPass)
    {
        return nullptr;
    }

    Ref<SamplerState> GLRenderAPI::CreateSampler(const SamplerSettings& settings)
    {
        return std::make_shared<GLSamplerState>(settings);
    }

    //TODO: Find better name or don't support it all together
    Ref<Renderbuffer> GLRenderAPI::CreateRenderBuffer(const RenderbufferSettings& settings)
    {
        return std::make_shared<GLRenderbuffer>(settings);
    }

    Ref<VertexBuffer> GLRenderAPI::CreateVertexBuffer(const void* data, uint32_t dataSize)
    {
        return API_ALLOC(GLVertexbuffer, data, dataSize);
    }

    Ref<VertexBuffer> GLRenderAPI::CreateVertexBuffer(uint32_t dataSize)
    {
        return API_ALLOC(GLVertexbuffer, dataSize);
    }

    Ref<VertexBufferArray> GLRenderAPI::CreateVertexBufferArray()
    {
        return API_ALLOC(GLVertexBufferArray);
    }

    Ref<PipelineState> GLRenderAPI::CreatePipelineState()
    {
        return API_ALLOC(GLPipelineState);
    }

    Ref<Swapchain> GLRenderAPI::CreateSwapChain(const SwapchainSettings& settings)
    {
        return API_ALLOC(GLSwapchain, m_Window);
    }

    Ref<RenderPass> GLRenderAPI::CreateRenderPass(const RenderPassSettings& settings)
    {
        return API_ALLOC(GLRenderPass, settings);
    }

    // void GLRenderAPI::BeginRenderPass(const Ref<RenderPass>& renderTarget)
    // {
    //     const auto glRT = std::reinterpret_pointer_cast<GLRenderPass>(renderTarget);
    //
    //     glRT->Bind();
    //
    // }
    //
    // void GLRenderAPI::EndRenderPass(const Ref<RenderPass>& renderTarget)
    // {
    //     const auto glRT = std::reinterpret_pointer_cast<GLRenderPass>(renderTarget);
    //
    //     glRT->UnBind();
    // }

    RenderAPIType GLRenderAPI::GetRenderAPIType()
    {
        
        return RenderAPIType::Opengl;
    }

    RenderAPIType GLRenderAPI::GetRenderAPIType() const
    {
        return RenderAPIType::Opengl;
    }

    GLRenderAPI::~GLRenderAPI()
    {
    }
}
