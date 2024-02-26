#include "renderer/graphic_context.hpp"

#include "core/assert.hpp"
#include "renderer/renderer_api.hpp"
#include "platform/OpenGL/opengl_graphic_context.hpp"

namespace acid
{

Ref<GraphicContext> GraphicContext::Create(GLFWwindow* handle)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::None: AC_ASSERT_MSG(false, "AcidGraphicAPI::None is not supported"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLGraphicContext>(handle);
    }
}

} // namespace acid