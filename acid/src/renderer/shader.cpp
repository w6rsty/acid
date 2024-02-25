#include "renderer/shader.hpp"

#include "core/assert.hpp"
#include "renderer/graphic_api.hpp"
#include "platform/OpenGL/opengl_shader.hpp"

namespace acid
{

Ref<Shader> Shader::Create(const std::string& filename)
{
    switch (AcidGraphicAPI)
    {
        case AcidGraphicAPI::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case AcidGraphicAPI::OpenGL: return CreateRef<OpenGLShader>(filename);
    }
}

Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
{
    switch (AcidGraphicAPI)
    {
        case AcidGraphicAPI::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case AcidGraphicAPI::OpenGL: return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc);
    }
}

} // namespace acid