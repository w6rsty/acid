#include "renderer/shader.hpp"

#include "core/assert.hpp"
#include "renderer/renderer_api.hpp"
#include "platform/OpenGL/opengl_shader.hpp"

namespace acid
{

Ref<Shader> Shader::Create(const std::string& filename)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(filename);
    }
}

Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc);
    }
}

} // namespace acid